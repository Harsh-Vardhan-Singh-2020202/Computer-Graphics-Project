// graphManager.h

# include "graphManager.h"
# include "edge.h"

// Function to create and add a new sketch to the "edges" list
void GraphManager::createAndAddEdge()
{
    Node* n1;
    Node* n2;
    int rel;
    edgeCount++;
    Edge* newEdge = new Edge(n1,n2,rel);
    newEdge->setId(edgeCount);
    EdgeList.push_back(newEdge);
}

// Function to delete the selected edges
void GraphManager::deleteSelectedEdges()
{
    if (selectedEdgeIndex >= 0 && selectedEdgeIndex < EdgeList.size())
    {
        // Delete the selected sketch
        delete EdgeList[selectedEdgeIndex];
        EdgeList.erase(EdgeList.begin() + selectedEdgeIndex);

        // Deselect any selected sketch
        selectedEdgeIndex = -1;
        edgeCount -= 1;
    }
}

void GraphManager::deleteSelectedNodes()
{
    if (selectedNodeIndex >= 0 && selectedNodeIndex < EdgeList.size())
    {
        // Delete the selected sketch
        delete NodeList[selectedNodeIndex];
        EdgeList.erase(EdgeList.begin() + selectedNodeIndex);

        // Deselect any selected sketch
        selectedNodeIndex = -1;
        nodeCount -= 1;
    }
}