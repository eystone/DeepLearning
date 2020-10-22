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

void InitGraph (Graph & graph, const struct SimpleCapParams & capture)
{
    for ( int i(0); i < capture.mHeight; i++ ) // init Node
        for ( int j(0); j < capture.mWidth; j++ )

            graph.addNode( unsigned( i * capture.mWidth + j ),                       //Ident
                           unsigned( capture.mTargetBuf[ i * capture.mWidth + j] )); //Binary


    for (unsigned i(0);i < graph.nmap.size(); i++) // init edge
    {
        graph.addEdge(i, (i - unsigned(capture.mWidth))-1 ); //Haut-gauche
        graph.addEdge(i, i -  unsigned(capture.mWidth) );    //Haut
        graph.addEdge(i, (i - unsigned(capture.mWidth))+1 ); //Haut-droite
        graph.addEdge(i,  i-1 );                             //gauche
        graph.addEdge(i,  i+1 );                             //droite
        graph.addEdge(i, (i + unsigned(capture.mWidth))-1 ); //bas-gauche
        graph.addEdge(i,  i + unsigned(capture.mWidth) );    //bas
        graph.addEdge(i, (i + unsigned(capture.mWidth))+1 ); //bas-droite
    }
}

int main()
{
    struct SimpleCapParams capture;
    capture.mWidth = 200;
    capture.mHeight = 150;
    capture.mTargetBuf = new int[ capture.mWidth * capture.mHeight ];

    if ( !InitCapture(capture) ) { return -1; }
    doCapture(0);

    Graph graph;
    InitGraph(graph, capture); // init nodes and edges

    //Test a Ascii art
    char light[] = " .,-o+O0@";

    // getMono = 0-255, on Decale de 5 bits et applique & 7 afin de passé a 0-7
    // utilise ces bits en tant qu'index dans notre chaine de car light
    for (unsigned i(0); i < graph.nmap.size(); i++)
    {
        std::cout << light[ 7 - (( graph.nmap[i]->getMono() >> 5) & 7) ];
        if (( i % unsigned( capture.mWidth ) ) == 0) std::cout << std::endl;
    }

    deinitCapture(0);
    return 0;
}
