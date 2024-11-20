// emissionProperties.h

#ifndef _EMISSIONPROPERTIES_H
#define _EMISSIONPROPERTIES_H

#include "utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class EmissionProperties
{
private:
    bool isDrawing = false;                             // Variables to track mouse input
    std::vector<float> drawingPoints;                   // Create a vector to store points of a single continuous stroke
    unsigned int VBO_drawingPoints;
    unsigned int VAO_drawingPoints;

public:
    EmissionProperties()
    {
        glGenBuffers(1, &VBO_drawingPoints);
        glGenVertexArrays(1, &VAO_drawingPoints);
    }

    void addPoint(std::vector<float> &points, float x, float y, int w, int h);
    void draw(ImGuiIO& io, int w, int h);
    void viewDrawing();

    void deleteVBO(){glDeleteBuffers(1, &VBO_drawingPoints);}

    // setters
    void setDrawingPoints(std::vector<float> dp){drawingPoints = dp;}

    // getters
    std::vector<float> getDrawingPoints(){return drawingPoints;}
};
#endif
