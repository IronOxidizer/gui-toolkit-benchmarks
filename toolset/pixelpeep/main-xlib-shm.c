/*
 * gcc main-xlib-shm.c -o pixelpeep-xlib-shm -I/usr/X11R6/include -L/usr/X11R6/lib -lX11 -lXext -Ofast
 * sudo apt-get install libxext-dev
 * 
 * 100,000 pixel fetches in 124s = 1.24ms per fetch
 * 
 * for some reason, the alpha channel is 255 when no window is on that pixel. This might be useful
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>
#include <unistd.h>

#define NAME  "screenshot"
#define BPP   4

struct shmimage
{
    XShmSegmentInfo shminfo ;
    XImage * ximage ;
    unsigned int * data ; // will point to the image's BGRA packed pixels
} ;

void initimage( struct shmimage * image )
{
    image->ximage = NULL ;
    image->shminfo.shmaddr = (char *) -1 ;
}

void destroyimage( Display * dsp, struct shmimage * image )
{
    if( image->ximage )
    {
        XShmDetach( dsp, &image->shminfo ) ;
        XDestroyImage( image->ximage ) ;
        image->ximage = NULL ;
    }

    if( image->shminfo.shmaddr != ( char * ) -1 )
    {
        shmdt( image->shminfo.shmaddr ) ;
        image->shminfo.shmaddr = ( char * ) -1 ;
    }
}

int createimage( Display * dsp, struct shmimage * image, int width, int height )
{
    // Create a shared memory area 
    image->shminfo.shmid = shmget( IPC_PRIVATE, width * height * BPP, IPC_CREAT | 0600 ) ;
    if( image->shminfo.shmid == -1 )
    {
        perror( NAME ) ;
        return false ;
    }

    // Map the shared memory segment into the address space of this process
    image->shminfo.shmaddr = (char *) shmat( image->shminfo.shmid, 0, 0 ) ;
    if( image->shminfo.shmaddr == (char *) -1 )
    {
        perror( NAME ) ;
        return false ;
    }

    image->data = (unsigned int*) image->shminfo.shmaddr ;
    image->shminfo.readOnly = false ;

    // Mark the shared memory segment for removal
    // It will be removed even if this program crashes
    shmctl( image->shminfo.shmid, IPC_RMID, 0 ) ;

    // Allocate the memory needed for the XImage structure
    image->ximage = XShmCreateImage( dsp, XDefaultVisual( dsp, XDefaultScreen( dsp ) ),
                        DefaultDepth( dsp, XDefaultScreen( dsp ) ), ZPixmap, 0,
                        &image->shminfo, 0, 0 ) ;
    if( !image->ximage )
    {
        destroyimage( dsp, image ) ;
        printf( NAME ": could not allocate the XImage structure\n" ) ;
        return false ;
    }

    image->ximage->data = (char *)image->data ;
    image->ximage->width = width ;
    image->ximage->height = height ;

    // Ask the X server to attach the shared memory segment and sync
    XShmAttach( dsp, &image->shminfo ) ;
    XSync( dsp, false ) ;
    return true ;
}

/*
void getrootwindow( Display * dsp, struct shmimage * image )
{
    XShmGetImage( dsp, XDefaultRootWindow( dsp ), image->ximage, 0, 0, AllPlanes ) ;
    // This is how you access the image's BGRA packed pixels
    // Lets set the alpha channel of each pixel to 0xff
    int x, y ;
    unsigned int * p = image->data ;
    for( y = 0 ; y < image->ximage->height; ++y )
    {
        for( x = 0 ; x < image->ximage->width; ++x )
        {
            *p++ |= 0xff000000 ;
        }
    }
}
*/

int main( int argc, char * argv[] ) {

    Display *dsp = XOpenDisplay(NULL) ;

    if( !XShmQueryExtension( dsp ) )
    {
        XCloseDisplay( dsp ) ;
        printf( NAME ": the X server does not support the XSHM extension\n" ) ;
        return 1 ;
    }

    int screen = XDefaultScreen( dsp ) ;
    struct shmimage image ;
    initimage( &image ) ;
    if( !createimage( dsp, &image, XDisplayWidth( dsp, screen ), XDisplayHeight( dsp, screen ) ) )
    {
        XCloseDisplay( dsp ) ;
        return 1 ;
    }

    //getrootwindow( dsp, &image ) ;
    for (int i = 0; i < 100000; ++i) {
        int y = 10, x = 10;
        XShmGetImage( dsp, XDefaultRootWindow( dsp ), image.ximage, 0, 0, AllPlanes ) ;
        //unsigned int pixel = XGetPixel(image.ximage, x, y); // for some reason this is wrong
        unsigned int pixel = image.data[y * image.ximage->width + x];
        printf("\n%u", pixel);
        sleep(0.1);
    }
    
    destroyimage( dsp, &image ) ;
    XCloseDisplay( dsp ) ;
    return 0 ;
}
