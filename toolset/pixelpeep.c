/*
 * This program monitors changes to a window and reports the time until the image has stabilized
 * 
 * compile dependencies: sudo apt-get install libxcb1-dev 
 * runtime dependencies: sudo apt-get install xcb
 * debug:   gcc -shared -o pixelpeep.so -fPIC -Og -Wall -g pixelpeep.c -lxcb -lxcb-shm 
 * release: gcc -shared -o pixelpeep.so -fPIC -Ofast pixelpeep.c -lxcb -lxcb-shm 
 * 
 * NOTE
 * - XYPixmap is 50x slower for some reason, so we use ZPixmap instead
 * - In the future, we should also try using the xcb damage extension
 * 
 * Benchmarks of various 500x400 image fetching approaches (smaller is better)
 * - 1.810ms C        XGetSubImage
 * - 1.130ms Python   XGetImage
 * - 1.030ms C        XGetImage
 * - 0.204ms C        xcb_image_get
 * - 0.063ms C        xcb_shm_get_image_unchecked
 * 
 * Example python usage:
 * 
 *   from ctypes import cdll
 *   pixelpeep = cdll.LoadLibrary("./pixelpeep.so")
 *   result = pixelpeep.await_stable_image(b"app-name", 0, 0)
 *   print("Result:", result)
 */

#include <xcb/xcb.h>
#include <xcb/xcb_image.h>
#include <xcb/shm.h>
#include <sys/shm.h>

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// debug dependencies
#include <stdio.h>
#include <png.h> // add -lpng

#define AllPlanes       ~0u // https://refspecs.linuxfoundation.org/LSB_5.0.0/LSB-Desktop-generic/LSB-Desktop-generic.html
#define BYTES_PER_PIXEL 4   // assume 4 bytes per pixel

void print_hex(const char *s) {
    fprintf(stderr,"\n");
    while (*s) fprintf(stderr, "%02x ", (unsigned int) *s++);
    fprintf(stderr,"\n");
}

xcb_window_t search_window_by_name(xcb_connection_t *conn, xcb_window_t window, const char *search_string) {
    // check name
    xcb_get_property_reply_t *name_reply = xcb_get_property_reply(conn, xcb_get_property(conn, 0, window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 0, 256), NULL);
    if (name_reply) {
        size_t name_len = xcb_get_property_value_length(name_reply);
        if (name_len > 1) {
            char *name = malloc(name_len + 1);
            memcpy(name, xcb_get_property_value(name_reply), name_len);
            name[name_len] = '\0';
            if (strstr(name, search_string)) {
                free(name_reply);
                free(name);
                return window;
            }
            free(name);
        }
        free(name_reply);
        
    }

    // get children
    xcb_query_tree_reply_t *tree_reply = xcb_query_tree_reply(conn, xcb_query_tree(conn, window), NULL);
    xcb_window_t *children = xcb_query_tree_children(tree_reply);
    size_t num_children = xcb_query_tree_children_length(tree_reply);

    // recurse
    for (size_t i = 0; i < num_children; ++i) {
        xcb_window_t child = search_window_by_name(conn, children[i], search_string);
        if (child != 0) {
            free(tree_reply);
            return child;
        }
    }
    
    // cleanup
    free(tree_reply);
    return 0;
}

// for debugging purposes only
void write_image_to_file(unsigned char* frame_buffer, int width, int height, char* filename) {
    FILE *fp = fopen(filename, "wb");
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    setjmp(png_jmpbuf(png_ptr));
    png_init_io(png_ptr, fp);
    png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(png_ptr, info_ptr);
    unsigned char **row_pointers = (unsigned char**) malloc(sizeof(unsigned char*) * height);
    for (int y = 0; y < height; y++) row_pointers[y] = &frame_buffer[y * width * 4];
    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, NULL);
    free(row_pointers);
    fclose(fp);
    png_destroy_write_struct(&png_ptr, &info_ptr);
}

/*
 * @param   char*   window_name the name of the window to monitor, if NULL, will monitor the entire screen
 * @param   uint    timeout     the max number of polls to do, where each poll takes ~1ms
 * @param   int     stabilizer  the min number of ticks required before the image is considered stable
 * @return  int                 time in milliseconds for the image to stabilize
 *                                  -1 if the timeout was reached
 *                                  -2 if the window could not be found
 *                                  -3 if the image never changed, this can happen if the window is not entirely in the screen
 */
int await_stable_image(const char *window_name, unsigned int timeout, unsigned int stabilizer) {
    // start timer
    struct timespec start_time, end_time;
    clock_gettime(1, &start_time);
    if (!timeout) timeout = 10000;      // default 10s
    if (!stabilizer) stabilizer = 1000; // default 1s
    
    // open the connection to the X server
    xcb_connection_t *conn = xcb_connect(NULL, NULL);
    xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;
    
    // get window
    xcb_window_t window;
    if (window_name) {
        window = search_window_by_name(conn, screen->root, window_name);
        if (!window) return -2;
        // find top level child (first child under root that contains the window)
        xcb_query_tree_reply_t *tree_reply = xcb_query_tree_reply(conn, xcb_query_tree(conn, window), NULL);
        xcb_window_t parent = tree_reply->parent;
        free(tree_reply);
        while (parent != screen->root) {
            window = parent;
            tree_reply = xcb_query_tree_reply(conn, xcb_query_tree(conn, window), NULL);
            parent = tree_reply->parent;
            free(tree_reply);
        }
    } else {
        window = screen->root;
    }
    
    // get window geometry
    xcb_get_geometry_reply_t *geometry_reply = xcb_get_geometry_reply(conn, xcb_get_geometry(conn, window), NULL);
    uint16_t w = geometry_reply->width, h = geometry_reply->height;
    int16_t x = geometry_reply->x, y = geometry_reply->y;
    free(geometry_reply);
    
    // setup shared memory
    xcb_shm_seg_t seg = xcb_generate_id(conn);
    size_t data_bytes = w * h * BYTES_PER_PIXEL;
    int shm_id = shmget(IPC_PRIVATE, data_bytes, IPC_CREAT | 0777);
    xcb_shm_attach(conn, seg, shm_id, 0);
    char *shared_buffer = (char *)shmat(shm_id, NULL, 0);
    char *image_copy = (char *)malloc(data_bytes);
    
    // time stable image
    for (int stable_ticks = 0; stable_ticks < stabilizer && --timeout; ++stable_ticks) {
        // sleep for 1ms to not load the CPU and influence the results
        usleep(1000); 
        
        // update shared buffer
        free(xcb_shm_get_image_reply(conn, xcb_shm_get_image_unchecked(
            conn, screen->root, x, y, w, h, AllPlanes, XCB_IMAGE_FORMAT_Z_PIXMAP, seg, 0), NULL));
        
        // reset if doesn't match
        if (memcmp(image_copy, shared_buffer, data_bytes)) {
            clock_gettime(1, &end_time);
            stable_ticks = 0;
            memcpy(image_copy, shared_buffer, data_bytes);
        }
    }
    
    //write_image_to_file((unsigned char *)image_copy, w, h, "test-image.png");
    
    // cleanup
    free(image_copy);
    xcb_shm_detach(conn, seg);
    shmdt(shared_buffer);
    xcb_disconnect(conn);
    
    // return results
    if (!timeout) return -1;
    int elapsed_time = (int)((end_time.tv_sec - start_time.tv_sec) * 1000 + (end_time.tv_nsec - start_time.tv_nsec) / 1e6);
    if (elapsed_time < 0) return -3;
    return elapsed_time;
}

int main() {
    await_stable_image("app-name", 0, 0);
    await_stable_image("app-name", 0, 0);
    await_stable_image("app-name", 0, 0);
    return 0;
}
