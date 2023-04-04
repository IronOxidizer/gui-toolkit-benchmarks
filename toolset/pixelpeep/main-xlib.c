/*
 * sudo apt install libx11-dev
 * gcc main-xlib.c -lX11 -Ofast -o pixelpeep-xlib
 * 
 * 100,000 pixel fetches in 157s = 1.57ms per fetch
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int x = 10, y = 10;
    
    Display *d = XOpenDisplay(NULL);
    Window window = XRootWindow(d, XDefaultScreen(d));
    
    XImage *image = XGetImage(d, window, x, y, 1, 1, AllPlanes, XYPixmap);
    XGetPixel(image, 0, 0);
    //sleep(0.1);
        
    for (int i = 0; i < 99999; ++i) {
        XGetSubImage(d, window, x, y, 1, 1, AllPlanes, XYPixmap, image, 0, 0);
        XGetPixel(image, 0, 0);
        //sleep(0.1);
    }
    
    XFree(image);
    return 0;
}
