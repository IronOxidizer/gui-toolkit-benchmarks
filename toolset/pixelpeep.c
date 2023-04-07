/*
 * dependencies: sudo apt install libx11-dev
 * debug: gcc -shared -o pixelpeep.so -fPIC -Og -Wall -g pixelpeep.c -lX11
 * release: gcc -shared -o pixelpeep.so -fPIC -Ofast pixelpeep.c -lX11
 * 
 * This program monitors color changes at a specific pixel
 * 
 * It can poll 10000000 pixel fetches in 137s, or 0.0137ms per fetch
 * NOTE:
 * - the XCB api was also tested and benchmarked, but was not any faster due to not having an alternative to the XGetSubImage API
 * - XYPixmap is 50x slower for some reason, so we use ZPixmap instead
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>

/*
 * @param   int     x       the x position of the pixel
 * @param   int     y       the y position of the pixel
 * @param   uint    timeout the max number of polls to do, where each poll takes ~1ms
 * @param   int     color   the color that the pixel must change to, any color change will end the timer if set to -1
 * @return  uint            1 if pixel changes, 0 if timed out
 */
unsigned int await_pixel_change(int x, int y, unsigned int timeout, int color) {
    // TODO: Setup boilerplate once
    Display *d = XOpenDisplay(NULL);
    Window window = XRootWindow(d, XDefaultScreen(d));
    XImage *image = XGetImage(d, window, x, y, 1, 1, AllPlanes, ZPixmap);
    
    unsigned int pixel = XGetPixel(image, 0, 0);
    unsigned int start_color = pixel;
    
    for (; timeout; --timeout) {
        if ((color == -1 && pixel != start_color) || pixel == color) break;
        usleep(1000); // Sleep 1ms to not put load on the CPU and influence the results
        XGetSubImage(d, window, x, y, 1, 1, AllPlanes, ZPixmap, image, 0, 0);
        pixel = XGetPixel(image, 0, 0);
    }
    
    // TODO: Do cleanup once
    XDestroyImage(image);
    XCloseDisplay(d);
    return timeout != 0;
}

/*
 * @param   int     x       the x position of the pixel
 * @param   int     y       the y position of the pixel
 * @param   [uint]  timeout (default=10,000,000) the max time in microseconds to wait for
 * @param   [uint]  color   the color that the pixel must change to, any color change will end the timer if un-specified
 * @return  int             the time, in microseconds, until the pixel change was detected, this is -1 when timed out
 * /
#define NOT_TIMED_OUT (now_time.tv_sec < endtime_s && now_time.tv_nsec < endtime_ns)
int poll_pixel(int x, int y, unsigned int timeout, unsigned int color) {
    struct timespec start_time, now_time;
    clock_gettime(1, &start_time);
    if (!timeout) timeout = 10000000;
    time_t endtime_s = start_time.tv_sec + timeout / 1000000;
    long endtime_ns = start_time.tv_nsec + (timeout % 1000000) * 1000;
    
    Display *d = XOpenDisplay(NULL);
    Window window = XRootWindow(d, XDefaultScreen(d));
    
    XImage *image = XGetImage(d, window, x, y, 1, 1, AllPlanes, ZPixmap);
    unsigned int pixel = XGetPixel(image, 0, 0);
    clock_gettime(1, &now_time);
    
    if (color) {    // detect when color matches
        while (pixel != color && NOT_TIMED_OUT) {
            XGetSubImage(d, window, x, y, 1, 1, AllPlanes, ZPixmap, image, 0, 0);
            pixel = XGetPixel(image, 0, 0);
            clock_gettime(1, &now_time);
        }
    } else {        // detect any color change
        color = pixel;
        while (pixel == color && NOT_TIMED_OUT) {
            XGetSubImage(d, window, x, y, 1, 1, AllPlanes, ZPixmap, image, 0, 0);
            pixel = XGetPixel(image, 0, 0);
            clock_gettime(1, &now_time);
        }
    }
    XDestroyImage(image);
    
    return NOT_TIMED_OUT
        ? (now_time.tv_sec - start_time.tv_sec) * 1000000 + (now_time.tv_nsec - start_time.tv_nsec) / 1000
        : -1;
}
*/
