#pragma once
#include "DataStructures.hpp"

void ParcourirGraph(std::map<Node*, bool>& visited, Node* node, std::string& Barcode) {
    visited[node] = true;
    for (std::pair<Node*, int> pairNode : node->getNeighbors()) {
        if (!visited[pairNode.first]) {

            Barcode += (pairNode.first->getMono() < 130) ? '1' : '0';

            ParcourirGraph(visited, pairNode.first, Barcode);
        }
    }
}

std::string ProcessBarcode(const std::string& Barcode) {
    std::string processedBarcode("");
    bool countB(false);
    unsigned count(0);
    for (unsigned i(0); i < Barcode.size(); i++) {
        if (Barcode[i] == '1') {
            countB = true;
            count += 1;
        }
        else if (countB) {
            processedBarcode += (count > 1) ? '1' : '0';
            countB = false;
            count = 0;
        }
    }
    return processedBarcode;
}

std::string ReadBarcode(Graph& graph, const unsigned& mW, const unsigned& mH) {
    Graph BarcodeGraph;

    //Create graph with draw Line
    for (unsigned i = 0; i < 4; i = i + 1) {
        for (unsigned x = 0; x < mW; x = x + 1) {
            BarcodeGraph.addNode(x + i * mW, graph.getNodes()[x + ((mH / 3) + i * 4) * mW]->getMono());
        }
    }

    for (Node* node : BarcodeGraph.getNodes()) {
        unsigned x = (node->getUId() % mW);
        unsigned y = (node->getUId() / mW);

        if ((x + 1) + y * mW < BarcodeGraph.getNodes()[BarcodeGraph.getNodes().size()-1]->getUId())
            BarcodeGraph.addEdge(x + y * mW, (x + 1) + y * mW);
    }
    //BarcodeGraph.printGraph();
    //BarcodeGraph.drawImage(mW);

    //Process graph to find result
    std::string Barcode;
    std::map<Node*, bool> visitedNodes;
    ParcourirGraph(visitedNodes, BarcodeGraph.getNodes()[0], Barcode);

    std::cout << Barcode << std::endl;

    return ProcessBarcode(Barcode);
}
