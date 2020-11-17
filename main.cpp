#include <iostream>

#include "escapi.h"
#include "DataStructures.hpp"
#include "GraphExplorer.hpp"

bool InitCapture(struct SimpleCapParams& capture) {
    int devices = setupESCAPI(); // init lib
    if (devices == 0) {
        std::cerr << "No webcam found" << std::endl;
        return false;
    }
    if (initCapture(0, &capture) == 0) {
        std::cerr << "Can't init Capture" << std::endl;
        return false;
    }
    for (int i(0); i < 5; i++) // do 5 loops for do light calib
    {
        doCapture(0);
        while (!isCaptureDone(0)) {} //Doing Capture         /!\ if webcam disconnect during this, INFINITE LOOP
    }
    return true;
}

void InitGraph(Graph& graph, const struct SimpleCapParams& capture) {

    for (int i(0); i < capture.mHeight; i++) {
        for (int j(0); j < capture.mWidth; j++) {
            unsigned mono = ConvertRGBBinToMono(capture.mTargetBuf[i * capture.mWidth + j]);
            graph.addNode(unsigned(i * capture.mWidth + j), mono);
        }
    }

    for (int i = 0; i < graph.getNodes().size(); i++) {
        unsigned x = (i % capture.mWidth);
        unsigned y = (i / capture.mWidth);

        // getIdWithCoord = (x + y * capture.mWidth);

        //!\\ NE PAS OULIER QUE C'EST UN GRAPH NON ORIENTE DONC TOUT LES VOISINS AUTOUR SONT PRIT
        if (y > 0) {
            graph.addEdge(i, x + (y - 1) * capture.mWidth); //node du bas
            if (x > 0)
                graph.addEdge(i, (x - 1) + (y - 1) * capture.mWidth); //node du bas droite
            if (x < capture.mWidth)
                graph.addEdge(i, (x + 1) + (y - 1) * capture.mWidth); //node du bas gauche
        }
        if (x > 0)
            graph.addEdge(i, (x - 1) + (y)*capture.mWidth); //node du milieu droit
    }
}

int main()
{
    
    struct SimpleCapParams capture;
    capture.mWidth = 150;
    capture.mHeight = 50;
    capture.mTargetBuf = new int[ capture.mWidth * capture.mHeight];

    if ( !InitCapture(capture) ) { return -1; }

    Graph graph;

    doCapture(0);
    
    InitGraph(graph, capture); // init nodes and edges

    deinitCapture(0);

   // "001100101110100100101";
    
    std::string result = ReadBarcode(graph, capture.mWidth, capture.mHeight);
    std::cout << result << std::endl;

    return 0;
}
