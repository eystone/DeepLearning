#pragma once
#include "DataStructures.hpp"

unsigned ParcourirGraph(std::map<Node*, bool>& visited, Node* node, unsigned valeur) {

    visited[node] = true;
    for (std::pair<Node*, int> pairNode : node->getNeighbors()) {

        if (!visited[pairNode.first]) {
            
            if (pairNode.second > 150 || pairNode.second < -150) valeur++;

            return ParcourirGraph(visited, pairNode.first, valeur);
        }
    }
    return valeur;
}

bool HaveBarcode(Graph& graph) {

    std::map<Node*, bool> visitedNodes;

    unsigned change = ParcourirGraph(visitedNodes, graph.getNodes()[0], 0);

    std::cout << "change: " << change << "size: " << graph.getNodes().size() << std::endl;
    
    //on regarde si le nombre de changement brutaux est equivalent a 15% des nodes du graph alors on considere que c'est un code barre
    return (change > graph.getNodes().size()*0.15) ? true : false;
}
