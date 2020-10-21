#include <iostream>

#include "escapi.h"

bool InitCapture (struct SimpleCapParams & capture)
{
    int devices = setupESCAPI(); // init lib
    if (devices == 0)
    {
        std::cerr << "No webcam found" << std::endl;
        return false;
    }
    if (initCapture(0, &capture) == 0)
    {
        std::cerr << "Can't init Capture" << std::endl;
        return false;
    }
    for (int i(0); i < 5; i++) // do 5 loops for do light calib
    {
        doCapture(0);
        while (!isCaptureDone(0)){} //Doing Capture         /!\ if webcam disconnect during this, INFINITE LOOP
    }
    return true;
}

int main()
{
    struct SimpleCapParams capture;
    capture.mWidth = 50;
    capture.mHeight = 20;
    capture.mTargetBuf = new int[50 * 20];

    if ( !InitCapture(capture) ) { return -1; }

    //Test a Ascii art
    char light[] = ".,-o+O0@";
    for (int i(0); i < 20; i++)
    {
        //">> 13" Decale de 13 Bits vers la droite afin de trouver les bits de vert
        //"& 7" met un filtre &0111 afin de garder que les bits que l'on a decalé
        // utilise ces bits en tant qu'index dans notre chaine de car light
        for (int j(0); j < 50; j++) printf("%c", light[(capture.mTargetBuf[i*50+j] >> 13) & 7]);
        std::cout << std::endl;
    }
    deinitCapture(0);
    return 0;
}
