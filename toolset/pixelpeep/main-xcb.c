/*
 * compile dependencies: sudo apt-get install libxcb1-dev libxcb-image0-dev
 * runtime dependencies: sudo apt-get install xcb
 * debug: gcc main-xcb.c -lxcb -lxcb-image -Og -Wall -o pixelpeep-xcb
 * release: gcc main-xcb.c -lxcb -lxcb-image -Ofast -o pixelpeep-xcb
 * 
 * This program monitors a specific pixel, and prints how many microseconds (0.000001) it takes for it to change color
 * 
 * 100,000 pixel fetches in 157s = 1.57ms per fetch
 * 
 * @param   int     x       the x position of the pixel
 * @param   int     y       the y position of the pixel
 * @param   [uint]  timeout (default=10,000,000) the max time in microseconds to wait for
 * @param   [uint]  color   the color that the pixel must change to, any color change will end the timer if un-specified
 * @return  int     {0, 1}  returns 0 if a change was detected before the timeout, 1 if the timeout was reached
 *
 * TODO: this should probably be a shared library that's callable from within our python script
 */

#include <xcb/xcb.h>
#include <xcb/xcb_image.h> 

#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>

// https://refspecs.linuxfoundation.org/LSB_5.0.0/LSB-Desktop-generic/LSB-Desktop-generic.html
#define AllPlanes   ((unsigned long)~0L)
#define XYPixmap    1

int main() {
    // Location of pixel to check
    int16_t x = 10, y = 10;
    
    // Open the connection to the X server. Use the DISPLAY environment variable
    int screen_idx;
    xcb_connection_t *conn = xcb_connect(NULL, &screen_idx);

    // Get the screen whose number is screen_idx
    const xcb_setup_t *setup = xcb_get_setup(conn);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);  

    // We want the screen at index screen_idx of the iterator
    for (int i = 0; i < screen_idx; ++i)
        xcb_screen_next(&iter);
    xcb_screen_t *screen = iter.data;
    
    // Get pixel
    for (int i = 0; i < 100000; ++i) {
        uint32_t pixel = xcb_image_get_pixel(xcb_image_get(conn, screen->root, x, y, 1, 1, AllPlanes, XYPixmap), 0, 0);
        //printf("\n%d", pixel);
        //sleep(0.1);
    }
    return 0;
}
