// emissionTextureManager.h

#ifndef _EMISSIONTEXTUREMANAGER_H
#define _EMISSIONTEXTUREMANAGER_H

#include "utils.h"
#include <vector>

class EmissionTexture;

class EmissionTextureManager
{
private:
    std::vector<EmissionTexture*> textures;
    int selectedTextureIndex;
    int textureCount;
    int trueTextureCount;

public:
    EmissionTextureManager()
    {
        // Initialize to no selected textures
        selectedTextureIndex = -1;
        // Initialize the textures count
        textureCount = 0;
        trueTextureCount = 0;
    }

    // Function to create and add a new textures to the "textures" list
    void createAndAddTextures();

    // Function to select a certain textures from the index
    void selectTextures(int index);

    // Function to draw the selected textures
    void drawSelectedTextures(ImGuiIO& io, int w, int h, double currentTime);

    // Function to view all the  textures
    void viewAllTextures(double currentTime);

    // Function to delete the selected textures
    void deleteSelectedTextures();

    // Function to delete VBOs of all textures
    void deleteAllVBOs();

    // setters
    void setTextureIndex(int index){selectedTextureIndex = index;}

    // getters
    std::vector<EmissionTexture*> getTextures(){return textures;}
    int getTextureIndex(){return selectedTextureIndex;}
    int getTexturesCount(){return textureCount;}
    int getTrueTexturesCount(){return trueTextureCount;}
};
#endif
