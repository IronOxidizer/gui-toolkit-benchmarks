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
 */

#include <xcb/xcb.h>
#include <xcb/xcb_image.h>
#include <xcb/shm.h>
#include <sys/shm.h>

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define AllPlanes       ~0u // https://refspecs.linuxfoundation.org/LSB_5.0.0/LSB-Desktop-generic/LSB-Desktop-generic.html
#define BYTES_PER_PIXEL 4   // assume 4 bytes per pixel

xcb_window_t search_window_by_name(xcb_connection_t *conn, xcb_window_t window, const char *search_string) {
    // check name
    xcb_get_property_reply_t *name_reply = xcb_get_property_reply(conn, xcb_get_property(conn, 0, window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 0, 256), NULL);
    if (name_reply) {
        char *name = xcb_get_property_value(name_reply);
		free(name_reply);
        if (name && strstr(name, search_string)) return window;
    }

    // get children
    xcb_query_tree_reply_t *tree_reply = xcb_query_tree_reply(conn, xcb_query_tree(conn, window), NULL);
    if (!tree_reply) return 0;
    xcb_window_t *children = xcb_query_tree_children(tree_reply);
    int num_children = xcb_query_tree_children_length(tree_reply);
    free(tree_reply);

    // recurse
    for (int i = 0; i < num_children; ++i) {
        xcb_window_t child = search_window_by_name(conn, children[i], search_string);
        if (child != 0) return child;
    }
    return 0;
}


/*
 * @param   char*   window_name the name of the window to monitor, if NULL, will monitor the entire screen
 * @param   uint    timeout     the max number of polls to do, where each poll takes ~1ms
 * @param   int     stabilizer  the min number of ticks required before the image is considered stable
 * @return  int                 time in milliseconds for the image to stabilize
 *                                  if the timeout was reached, this will return -1
 *                                  if the window could not be found, this will return -2
 */
int await_stable_image(char *window_name, unsigned int timeout, unsigned int stabilizer) {
    // start timer
    struct timespec start_time, end_time;
    clock_gettime(1, &start_time);
    
    if (!timeout) timeout = 10000;      // default 10s
    if (!stabilizer) stabilizer = 1000; // default 1s
    
    // open the connection to the X server
    xcb_connection_t *conn = xcb_connect(NULL, NULL);
    xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;
    
    // get window and properties
    xcb_window_t window = window_name ? search_window_by_name(conn, screen->root, window_name) : screen->root;
    if (!window) return 1;
    xcb_get_geometry_reply_t *geometry_reply = xcb_get_geometry_reply(conn, xcb_get_geometry(conn, window), NULL);
    uint16_t w = geometry_reply->width, h = geometry_reply->height;
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
        xcb_shm_get_image_reply(conn, xcb_shm_get_image_unchecked(
            conn, screen->root, 0, 0, w, h, AllPlanes, XCB_IMAGE_FORMAT_Z_PIXMAP, seg, 0), NULL);
        
        // reset if doesn't match
        if (memcmp(image_copy, shared_buffer, data_bytes)) {
            clock_gettime(1, &end_time);
            fprintf(stderr, "Change detected");
            stable_ticks = 0;
            memcpy(image_copy, shared_buffer, data_bytes);
        }
    }
    
    // cleanup
    free(image_copy);
    xcb_shm_detach(conn, seg);
    shmdt(shared_buffer);
    xcb_disconnect(conn);
    
    // return results
    if (!timeout) return -1;
    int elapsed_time = (int)((end_time.tv_sec - start_time.tv_sec) * 1000 + (end_time.tv_nsec - start_time.tv_nsec) / 1e6);
    return elapsed_time;
}
