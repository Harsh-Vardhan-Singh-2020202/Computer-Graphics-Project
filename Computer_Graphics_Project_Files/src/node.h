// node.h

#ifndef SRC_NODE_H
#define SRC_NODE_H

#include <string>

class Sketching;
class EmissionTexture;

class Node
{
private:

public:
    int nodeType;
    Sketching* NodeType1;
    EmissionTexture* NodeType2;

    Node(int n_type, Sketching* n1 = NULL, EmissionTexture* n2 = NULL)
    {
        nodeType = n_type;

        if(n_type == 1)
        {
            NodeType1 = n1;
            NodeType2 = NULL;
        }
        else
        {
            NodeType1 = NULL;
            NodeType2 = n2;
        }
    }


    int getNodeType(){return nodeType;}
};
#endif
