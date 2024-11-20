// objectManager.cpp

#include "objectManager.h"
#include "sketching.h"
#include <iostream>

// Function to create and add a new sketch to the "objects" list
void ObjectManager::createAndAddSketch()
{
    sketchCount++;
    trueSketchCount++;
    Sketching* newSketch = new Sketching(); // Create a new Sketching object
    newSketch->setId(trueSketchCount);
    objects.push_back(newSketch); // Add it to the objects list
}

// Function to select a certain object from the index
void ObjectManager::selectObject(int index)
{
    if (index >= 0 && index < objects.size())
    {
        selectedObjectIndex = index; // Set the selected object index
    }
}

// Function to draw the selected object
void ObjectManager::drawSelectedObject(ImGuiIO& io, int w, int h)
{
    if (selectedObjectIndex >= 0 && selectedObjectIndex < objects.size())
    {
        objects[selectedObjectIndex]->draw(io, w, h); // Draw the selected object
    }
}

// Function to draw the selected object
void ObjectManager::viewAllObject()
{
    for (int i = 0; i < objects.size(); i++)
    {
        if(i != selectedObjectIndex)
        {
            objects[i]->viewDrawing(); // View the selected object
        }
    }
}

// Function to delete the selected sketch
void ObjectManager::deleteSelectedSketch()
{
    if (selectedObjectIndex >= 0 && selectedObjectIndex < objects.size())
    {
        // Delete the selected sketch
        delete objects[selectedObjectIndex];
        objects.erase(objects.begin() + selectedObjectIndex);

        // Deselect any selected sketch
        selectedObjectIndex = -1;
        sketchCount -= 1;
    }
}

// Function to delete VBOs of all sketches
void ObjectManager::deleteAllVBOs()
{
    for (Sketching* sketch : objects)
    {
        sketch->deleteVBO(); // Call the deleteVBO function for each sketch
    }
}