// emissionTextureManager.cpp

#include "emissionTextureManager.h"
#include "emissionTexture.h"
#include "emissionProperties.h"
#include <iostream>

// Function to create and add a new texture to the "textures" list
void EmissionTextureManager::createAndAddTextures()
{
    textureCount++;
    trueTextureCount++;

    EmissionProperties em;
    EmissionProperties es;
    EmissionProperties ep;
    EmissionProperties s;
    EmissionTexture* newTexture = new EmissionTexture(s,em, es, ep, 1.0f, 1.0f); // Create a new texture
    newTexture->setId(trueTextureCount);
    textures.push_back(newTexture); // Add it to the textures list
}

// Function to select a certain textures from the index
void EmissionTextureManager::selectTextures(int index)
{
    if (index >= 0 && index < textures.size())
    {
        selectedTextureIndex = index; // Set the selected textures index
    }
}

// Function to draw the selected textures
void EmissionTextureManager::drawSelectedTextures(ImGuiIO& io, int w, int h, double currentTime)
{
    if (selectedTextureIndex >= 0 && selectedTextureIndex < textures.size())
    {
        textures[selectedTextureIndex]->draw(io, w, h, currentTime); // Draw the selected object
    }
}

// Function to draw the selected textures
void EmissionTextureManager::viewAllTextures(double currentTime)
{
    for (int i = 0; i < textures.size(); i++)
    {
        if(i != selectedTextureIndex)
        {
            textures[i]->viewDrawing(currentTime); // View the selected object
        }
    }
}

// Function to delete the selected texture
void EmissionTextureManager::deleteSelectedTextures()
{
    if (selectedTextureIndex >= 0 && selectedTextureIndex < textures.size())
    {
        // Delete the selected texture
        delete textures[selectedTextureIndex];
        textures.erase(textures.begin() + selectedTextureIndex);

        // Deselect any selected texture
        selectedTextureIndex = -1;
        textureCount -= 1;
    }
}

// Function to delete VBOs of all textures
void EmissionTextureManager::deleteAllVBOs()
{
    for (EmissionTexture* texture : textures)
    {
        texture->deleteVBO(); // Call the deleteVBO function for each texture
    }
}