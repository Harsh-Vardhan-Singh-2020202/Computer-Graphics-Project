// objectManager.h

#ifndef _OBJECTMANAGER_H_
#define _OBJECTMANAGER_H_

#include "utils.h"
#include "sketching.h"
#include <vector>

class Sketching;

class ObjectManager
{
private:
    std::vector<Sketching*> objects; // Store pointers to Sketching instances
    int selectedObjectIndex; // Index of the currently selected object
    int sketchCount; // To keep track of the number of sketches
    int trueSketchCount; // To keep track of the number of sketches

public:
    ObjectManager()
    {
        // Initialize to no selected object
        selectedObjectIndex = -1;
        // Initialize the sketch count
        sketchCount = 0;
        trueSketchCount = 0;
    }

    // Function to create and add a new sketch to the "objects" list
    void createAndAddSketch();

    // Function to select a certain object from the index
    void selectObject(int index);

    // Function to draw the selected object
    void drawSelectedObject(ImGuiIO& io, int w, int h);

    // Function to view all the  object
    void viewAllObject();

    // Function to delete the selected sketch
    void deleteSelectedSketch();

    // Function to delete VBOs of all sketches
    void deleteAllVBOs();

    // setters
    void setSketchIndex(int index){selectedObjectIndex = index;}

    // getters
    std::vector<Sketching*> getObjects(){return objects;}
    int getSketchIndex(){return selectedObjectIndex;}
    int getObjectCount(){return sketchCount;}
    int getTrueObjectCount(){return trueSketchCount;}
};
#endif
