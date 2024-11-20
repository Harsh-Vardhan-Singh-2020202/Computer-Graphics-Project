// edge.h

#ifndef SRC_EDGE_H
#define SRC_EDGE_H

# include "node.h"
# include "edge.h"
#include <vector>
#include <iostream>

class Edge
{
private:
    int id;
    int relation; //1:linear, 2:square, 3:cubic, 4:exponential
    int edgeCount;
    int CreationLevel = 0;

public:
    Node* Node1;
    Node* Node2;

    Edge(Node* n1, Node* n2, int rel)
    {
        Node1 = n1;
        Node2 = n2;
        relation = rel;
    }

    // setters
    void setId(int _id){id = _id;}
    void setRelation(int _rel){relation = _rel;}
    void setNode1(Node* _no1){Node1 = _no1;}
    void setNode2(Node* _no2){Node2 = _no2;}
    void setCreationLevel(int c_l){CreationLevel = c_l;}

    // getters
    int getId(){return id;}
    int getCreationLevel(){return CreationLevel;}

};
#endif