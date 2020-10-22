#include <iostream>

#include "escapi.h"
#include "DataStructures.hpp"

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

int main()
{
    struct SimpleCapParams capture;
    capture.mWidth = 100;
    capture.mHeight = 50;
    capture.mTargetBuf = new int[ capture.mWidth * capture.mHeight ];

    if ( !InitCapture(capture) ) { return -1; }

    //Test a Ascii art
    char light[] = " .,-o+O0@";
    Graph graph;

    for ( int i(0); i < capture.mHeight; i++ )
    {
        // Pix.Mono = 0-255, on Decale de 5 bits et applique & 7 afin de passé a 0-7
        // utilise ces bits en tant qu'index dans notre chaine de car light
        for ( int j(0); j < capture.mWidth; j++ )
        {
            graph.addNode( unsigned(i * capture.mWidth + j), unsigned(capture.mTargetBuf[ i * capture.mWidth + j ]) );

            std::cout << light[7- ( ( graph.nmap[ unsigned(i * capture.mWidth + j) ]->getMono() >> 5) & 7)];
        }
        std::cout << std::endl;
    }
    deinitCapture(0);
    return 0;
}
