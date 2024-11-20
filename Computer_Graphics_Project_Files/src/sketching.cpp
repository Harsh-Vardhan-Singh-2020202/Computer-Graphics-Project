// sketching.cpp

#include "sketching.h"
#include "animating.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

extern bool sketchMode;

// Function to print a glm::mat3 matrix
void printMatrix(glm::mat3& matrix)
{
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Sketching::addPoint(std::vector<float> &points, float x, float y, int w, int h)
{
    float rescaled_x = -1.0 + ((1.0*x - 0) / (w - 0)) * (1.0 - (-1.0));
    float rescaled_y = -1.0 + ((1.0*(h - y) - 0) / (h - 0)) * (1.0 - (-1.0));
    points.push_back(rescaled_x);   // X-coordinate
    points.push_back(rescaled_y);   // Y-coordinate
    points.push_back(0.0);          // Z-coordinate
}

void Sketching::draw(ImGuiIO& io, int w, int h)
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

        // Create a new segment when starting to draw
        drawingSegments.push_back(drawingPoints);
        // Clear the current segment
        drawingPoints.clear();
    }


    if (isDrawing && sketchMode && ImGui::IsMouseDragging(0))
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

    // Render the drawingSegments
    for (const auto& segment : drawingSegments)
    {
        // Set the line width and render the line segment
        glBindBuffer(GL_ARRAY_BUFFER, VBO_drawingPoints);
        glBufferData(GL_ARRAY_BUFFER, segment.size() * sizeof(GLfloat), segment.data(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(VAO_drawingPoints);
        glDrawArrays(GL_LINE_STRIP, 0, segment.size() / 3);
    }

    // Draw drawing points
    glBindVertexArray(VAO_drawingPoints);
    glDrawArrays(GL_LINE_STRIP, 0, drawingPoints.size() / 3);
}

void Sketching::viewDrawing()
{
    // Render the drawingSegments
    for (const auto& segment : drawingSegments)
    {
        // Set the line width and render the line segment
        glBindBuffer(GL_ARRAY_BUFFER, VBO_drawingPoints);
        glBufferData(GL_ARRAY_BUFFER, segment.size() * sizeof(GLfloat), segment.data(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(VAO_drawingPoints);
        glDrawArrays(GL_LINE_STRIP, 0, segment.size() / 3);
    }

    // Draw drawing points
    glBindVertexArray(VAO_drawingPoints);
    glDrawArrays(GL_LINE_STRIP, 0, drawingPoints.size() / 3);
}

void Sketching::editDrawing(glm::mat3 t_mat)
{
    //printMatrix(t_mat);

    // Apply the transformations to the drawing segments
    for (std::vector<float>& segment : drawingSegments)
    {
        for (size_t i = 0; i < segment.size(); i += 3)
        {
            glm::vec3 point = glm::vec3(segment[i], segment[i + 1], 1.0f) * t_mat;
            segment[i] = point.x;
            segment[i + 1] = point.y;
            segment[i + 2] = point.z;
        }
    }
}

glm::mat3 Sketching::move(glm::mat3 t_mat, float dx, float dy)
{
    // Create a translation matrix for the current move
    glm::mat3 translationMatrix = glm::mat3(1.0f); // Identity matrix
    translationMatrix[0][2] = dx; // Set the translation in X
    translationMatrix[1][2] = dy; // Set the translation in Y

    // Apply the translation to the existing transformation matrix
    t_mat = translationMatrix * t_mat;

    return t_mat;
}

glm::mat3 Sketching::rotate(glm::mat3 t_mat, float angle, float px, float py)
{
    glm::mat3 rotationMatrix = glm::mat3(1.0f);
    rotationMatrix[0][2] = -px;
    rotationMatrix[1][2] = -py;

    // Calculate the cosine and sine of the rotation angle (in radians)
    float angleRadians = glm::radians(angle);
    float cosAngle = cos(angleRadians);
    float sinAngle = sin(angleRadians);

    // Apply the rotation
    rotationMatrix[0][0] = cosAngle;
    rotationMatrix[0][1] = -sinAngle;
    rotationMatrix[1][0] = sinAngle;
    rotationMatrix[1][1] = cosAngle;

    rotationMatrix[0][2] = px;
    rotationMatrix[1][2] = py;
    // Apply the rotation to the existing transformation matrix
    t_mat = glm::mat3(rotationMatrix) * t_mat;

    return t_mat;
}

glm::mat3 Sketching::scale(glm::mat3 t_mat, float sx, float sy)
{
    glm::mat3 scaleMatrix = glm::mat3(1.0f); // Identity matrix
    scaleMatrix[0][0] = scaleMatrix[0][0] * sx; // Scale factor in X
    scaleMatrix[1][1] = scaleMatrix[1][1] * sy; // Scale factor in Y

    // Apply the scaling to the existing transformation matrix
    t_mat = scaleMatrix * t_mat;

    return t_mat;
}

// Function to create and add a new sketch to the "objects" list
void Sketching::createAndAddAnimation()
{
    animCount++;
    trueAnimCount++;
    Animating* newAnim = new Animating(); // Create a new Sketching object
    newAnim->setId(trueAnimCount);
    animatingList.push_back(newAnim); // Add it to the objects list
}

void Sketching::selectAnimation(int index)
{
    if (index >= 0 && index < animatingList.size())
    {
        selectedAnimIndex = index;
    }
}

void Sketching::deleteSelectedAnim()
{
    if (selectedAnimIndex >= 0 && selectedAnimIndex < animatingList.size())
    {
        // Delete the selected sketch
        delete animatingList[selectedAnimIndex];
        animatingList.erase(animatingList.begin() + selectedAnimIndex);

        // Deselect any selected sketch
        selectedAnimIndex = -1;
        animCount -= 1;
    }
}

void Sketching::animateObject(Animating* selectedAnimation, glm::mat3 animMat, float dx, float dy, float ang, float px, float py)
{
    if (selectedAnimation->getIsPlaying())
    {
        glm::mat3 matAnim = animMat;

        // Calculate the transformation matrix at the current time
        float anim_duration = selectedAnimation->getAnimationDuration();
        float _dx     = (dx/(ImGui::GetIO().Framerate * anim_duration));
        float _dy     = (dy/(ImGui::GetIO().Framerate * anim_duration));
        float _ang    = (ang/(ImGui::GetIO().Framerate * anim_duration));

        matAnim = move(matAnim, _dx, _dy);
        matAnim = rotate(matAnim, _ang, px, py);

        editDrawing(matAnim);

        // Increment the animation time based on the frame rate
        float a_t = selectedAnimation->getAnimationTime();
        selectedAnimation->setAnimationTime(a_t += 1/ImGui::GetIO().Framerate);

        // Check if the animation is finished and decide what to do next (loop, stop, etc.)
        if (selectedAnimation->getAnimationTime() >= selectedAnimation->getAnimationDuration())
        {
            if (selectedAnimation->getIsLooping())
            {
                selectedAnimation->setAnimationTime(0.0f);

                // reset position
                matAnim = move(matAnim, -dx, -dy);
                matAnim = rotate(matAnim, -ang, px, py);
                editDrawing(matAnim);

            }
            else
            {
                selectedAnimation->setIsPlaying(false);
                selectedAnimation->setAnimationTime(0.0f);
            }
        }

    }
}

void Sketching::resetAnimation(Animating* selectedAnimation ,glm::mat3 animMat, float animTime, float animDur, float dx, float dy, float ang, float px, float py)
{
    glm::mat3 matAnim = animMat;

    if(!selectedAnimation->getIsPlaying())
    {
        selectedAnimation->setAnimationTime(0.0f);
        matAnim = move(matAnim, -dx * animTime/animDur, -dy * animTime/animDur);
        matAnim = rotate(matAnim, -ang * animTime/animDur, px, py);
        editDrawing(matAnim);
        selectedAnimation->setIsPlaying(false);
    }
}