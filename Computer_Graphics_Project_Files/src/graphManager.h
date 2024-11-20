// graphManager.h

#ifndef SRC_GRAPHMANAGER_H
#define SRC_GRAPHMANAGER_H

# include "node.h"
# include "edge.h"
#include <vector>
#include <iostream>

class GraphManager
{
private:
    std::vector<Node*> NodeList;
    std::vector<Edge*> EdgeList;

public:
    int edgeCount = 0;
    int nodeCount = 0;
    int selectedEdgeIndex = -1;
    int selectedNodeIndex = -1;

    GraphManager()
    {}

    void AddSketchToList(Node* node){NodeList.push_back(node);}
    void AddEdgeToList(Edge* edge){EdgeList.push_back(edge);}

    void createAndAddEdge();
    void deleteSelectedEdges();
    void deleteSelectedNodes();

    // getters
    std::vector<Node*> GetNodeList(){return NodeList;}
    std::vector<Edge*> GetEdgeList(){return EdgeList;}
};
#endif