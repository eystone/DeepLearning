#include <iostream>

#include "escapi.h"

struct RGBtoMono
{
    unsigned R;
    unsigned G;
    unsigned B;
    unsigned Mono;
};

bool InitCapture ( struct SimpleCapParams & capture )
{
    int devices = setupESCAPI(); // init lib
    if ( devices == 0 )
    {
        std::cerr << "No webcam found" << std::endl;
        return false;
    }
    if ( initCapture(0, &capture) == 0 )
    {
        std::cerr << "Can't init Capture" << std::endl;
        return false;
    }
    for ( int i(0); i < 5; i++ ) // do 5 loops for do light calib
    {
        doCapture(0);
        while ( !isCaptureDone(0) ) {} //Doing Capture         /!\ if webcam disconnect during this, INFINITE LOOP
    }
    return true;
}

void ConvertRGBtoMono ( RGBtoMono & Pix, const int & PixByte )
{
    // R=bits 16-24
    Pix.R = ((PixByte >> 16) & 255);
    // B=bits 8-16
    Pix.B = ((PixByte >> 8) & 255);
    // G=bits 0-8
    Pix.G = (PixByte & 255);

    //coeff based on what is most perceived by humans as color in one channel
    // http://poynton.ca/notes/colour_and_gamma/ColorFAQ.html#RTFToC9
    Pix.Mono = (0.2125 * Pix.R) + (0.7154 * Pix.G) + (0.0721 * Pix.B);
}

int main()
{
    struct SimpleCapParams capture;
    capture.mWidth = 256;
    capture.mHeight = 144;
    capture.mTargetBuf = new int[ capture.mWidth * capture.mHeight ];

    if ( !InitCapture(capture) ) { return -1; }


    //Test a Ascii art
    char light[] = " .,-o+O0@";
    RGBtoMono Pix;

    for ( int i(0); i < capture.mHeight; i++ )
    {
        // Pix.Mono = 0-255, on Decale de 5 bits et applique & 7 afin de passé a 0-7
        // utilise ces bits en tant qu'index dans notre chaine de car light
        for ( int j(0); j < capture.mWidth; j++ )
        {
            ConvertRGBtoMono( Pix, capture.mTargetBuf[ i * capture.mWidth + j ]);

            //std::cout << "R:" << Pix.R << " B:" << Pix.B << " G:" << Pix.G << " Mono:" << Pix.Mono << '\t';
            std::cout << light[(Pix.Mono >> 5) & 7];
        }
        std::cout << std::endl;
    }
    deinitCapture(0);
    return 0;
}
