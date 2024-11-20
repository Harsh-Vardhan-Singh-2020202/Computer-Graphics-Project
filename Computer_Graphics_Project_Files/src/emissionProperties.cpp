// emissionProperties.cpp

# include "emissionProperties.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

extern bool dynamicSketchMode;

void EmissionProperties::addPoint(std::vector<float> &points, float x, float y, int w, int h)
{
    float rescaled_x = -1.0 + ((1.0*x - 0) / (w - 0)) * (1.0 - (-1.0));
    float rescaled_y = -1.0 + ((1.0*(h - y) - 0) / (h - 0)) * (1.0 - (-1.0));
    points.push_back(rescaled_x);   // X-coordinate
    points.push_back(rescaled_y);   // Y-coordinate
    points.push_back(0.0);          // Z-coordinate
}

void EmissionProperties::draw(ImGuiIO& io, int w, int h)
{
    float x, y, x_prev, y_prev;

    if (ImGui::IsMouseClicked(0))
    {
        // Left mouse button pressed, start drawing
        isDrawing = true;
    }

    if (ImGui::IsMouseReleased(0))
    {
        // Left mouse button released, stop drawing
        isDrawing = false;
    }


    if (isDrawing && dynamicSketchMode && ImGui::IsMouseDragging(0))
    {
        // Left mouse button is held and mouse is being dragged
        x_prev = x;
        y_prev = y;
        x = io.MousePos.x;
        y = io.MousePos.y;
        if(x_prev != x || y_prev != y)
        {
            addPoint(drawingPoints, x, y, w, h);
        }
    }

    // Update VAO/VBO for drawing points (since we add a new point each time this condition is called)
    glBindVertexArray(VAO_drawingPoints);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_drawingPoints);
    glBufferData(GL_ARRAY_BUFFER, drawingPoints.size() * sizeof(GLfloat), &drawingPoints[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // Enable first attribute buffer (vertices)
    // Draw drawing points
    glBindVertexArray(VAO_drawingPoints);
    glDrawArrays(GL_LINE_STRIP, 0, drawingPoints.size() / 3);
}

void EmissionProperties::viewDrawing()
{
    // Update VAO/VBO for drawing points (since we add a new point each time this condition is called)
    glBindVertexArray(VAO_drawingPoints);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_drawingPoints);
    glBufferData(GL_ARRAY_BUFFER, drawingPoints.size() * sizeof(GLfloat), &drawingPoints[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // Enable first attribute buffer (vertices)
    // Draw drawing points
    glBindVertexArray(VAO_drawingPoints);
    glDrawArrays(GL_LINE_STRIP, 0, drawingPoints.size() / 3);
}