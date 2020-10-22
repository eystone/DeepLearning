#include <vector>
#include <map>

#ifndef DATASTRUCTURES_HPP
#define DATASTRUCTURES_HPP

struct Pix
{
    unsigned R;
    unsigned G;
    unsigned B;
    unsigned Mono; // 0=White / 255=Black

    Pix (const unsigned & PixByte );
};

Pix::Pix( const unsigned & PixByte )
{
    //DECODING RGB24
    // R=bits 16-24
    R = ( (PixByte >> 16) & 255);
    // G=bits 8-16
    G = ( (PixByte >> 8) & 255);
    // B=bits 0-8
    B = (PixByte & 255);

    //coeff based on what is most perceived as brightness by humans as color in one channel
    // http://poynton.ca/notes/colour_and_gamma/ColorFAQ.html#RTFToC9
    Mono = unsigned( (0.2125 * R) + (0.7154 * G) + (0.0721 * B) );
}


class Node {

    unsigned Ident;
    Pix Data;

public:
    typedef std::pair <int, Node*> Cost;
    std::vector<Cost> Edges;

    Node(const unsigned & i, const unsigned & binary) : Ident(i), Data(binary) {}

    unsigned getMono() const  { return Data.Mono; }
    unsigned getIdent() const { return Ident; }
};

class Graph
{
public:
    typedef std::map<unsigned, Node*> Nmap;
    Nmap nmap;

    Graph() {}

    bool nodeIsExist (const unsigned & ident);
    void addNode(const unsigned & ident, const unsigned & binary);
    void addEdge (const unsigned & from, const unsigned & to); // compute cost
};

bool Graph::nodeIsExist(const unsigned & ident)
{
    Nmap::iterator itr = nmap.find(ident);
    return itr != nmap.end(); // if not find, not exist so if different exist
}

void Graph::addNode(const unsigned & ident, const unsigned & binary)
{
    if (nodeIsExist(ident)) return;
    Node *n;
    n = new Node (ident, binary);
    nmap[ident] = n;
    return;
}

typedef std::pair <int, Node*> Cost;
void Graph::addEdge(const unsigned & from, const unsigned & to)
{
    if (!nodeIsExist(to)) return;
    Node *f = (nmap.find(from)->second);
    Node *t = (nmap.find(to)->second);

    //proc cost
    int cost = int(f->getMono()) - int(t->getMono()); //if brighter pos value else neg value

    Cost edge = std::make_pair(cost, t);
    f->Edges.push_back(edge);
}

#endif // DATASTRUCTURES_HPP
