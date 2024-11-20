// sketching.h

#ifndef _SKETCHING_H_
#define _SKETCHING_H_

#include "utils.h"
#include "animating.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Animating;

class Sketching
{
private:
    int id;
    bool isDrawing = false;                             // Variables to track mouse input
    std::vector<float> drawingPoints;                   // Create a vector to store points of a single continuous stroke
    std::vector<std::vector<float>> drawingSegments;    // Create a vector of vectors to store line segments
    unsigned int VBO_drawingPoints;
    unsigned int VAO_drawingPoints;

    glm::mat3 editTransformationMatrix;

    std::vector<Animating*> animatingList;
    int selectedAnimIndex;
    int animCount;
    int trueAnimCount;

public:
    Sketching()
    {
        selectedAnimIndex = -1;
        animCount = 0;
        trueAnimCount = 0;
        glGenBuffers(1, &VBO_drawingPoints);
        glGenVertexArrays(1, &VAO_drawingPoints);
        editTransformationMatrix = glm::mat3(1.0f);
    }

    void addPoint(std::vector<float> &points, float x, float y, int w, int h);
    void draw(ImGuiIO& io, int w, int h);
    void viewDrawing();
    void editDrawing(glm::mat3 t_mat);
    glm::mat3 move(glm::mat3 t_mat, float dx, float dy);
    glm::mat3 rotate(glm::mat3 t_mat, float angle, float px, float py);
    glm::mat3 scale(glm::mat3 t_mat, float sx, float sy);
    void animateObject(Animating* selectedAnimation, glm::mat3 animMat, float dx, float dy, float ang, float px, float py);
    void resetAnimation(Animating* selectedAnimation, glm::mat3 animMat, float animTime, float animDur, float dx, float dy, float ang, float px, float py);
    void deleteVBO(){glDeleteBuffers(1, &VBO_drawingPoints);}

    void createAndAddAnimation();
    void selectAnimation(int index);
    void deleteSelectedAnim();

    // setters
    void setId(int _id){id = _id;}

    // getters
    int getId(){return id;}
    std::vector<Animating*> getAnimatingList(){return animatingList;}
    int getSelectedAnimIndex(){return selectedAnimIndex;}
    int getAnimCount(){return animCount;}
    int getTrueAnimCount(){return trueAnimCount;}
    glm::mat3 getEditTransformationMatrix(){return editTransformationMatrix;}
};
#endif