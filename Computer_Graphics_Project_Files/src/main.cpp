// main.cpp

#include "utils.h"
#include "objectManager.h"
# include "emissionTextureManager.h"
# include "graphManager.h"
# include "sketching.h"


#include <iostream>

// Width and Height of window
int width = 1600, height = 800;

// Checking modes
bool sketchMode;
bool dynamicSketchMode;
bool editMode;
bool animMode;
bool graphMode;

// Define and set the default color
ImVec4 selectedColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

// Main Driver Code
int main(int, char* argv[])
{
    glfwInit();

    // Setting up window and creating IO object
    GLFWwindow* window = setupWindow(width, height);
    ImGuiIO& io = ImGui::GetIO();

    // Setting the window color
    ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Setting up the shaders
    unsigned int shaderProgram = createProgram("./shaders/vshader.vs", "./shaders/fshader.fs");
    glUseProgram(shaderProgram);

    // Get the location of the selectedColor uniform
    GLint selectedColor_uniform = glGetUniformLocation(shaderProgram, "selectedColor");
    //Check if the uniform location is valid
    if (selectedColor_uniform == -1)
    {
        fprintf(stderr, "Could not bind location: selectedColor\n");
        exit(0);
    }

    // Create an instance of the object manager class
    ObjectManager objectManager;
    EmissionTextureManager emissionTextureManager;
    GraphManager graphManager;

    //Display loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        double currentTime = glfwGetTime();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Rendering
        glUseProgram(shaderProgram); // Activate the shader program
        glUniform4f(selectedColor_uniform, selectedColor.x, selectedColor.y, selectedColor.z, selectedColor.w);
        glUseProgram(0);

        showOptionsDialog(io, objectManager, emissionTextureManager, graphManager, shaderProgram, selectedColor_uniform);
        ImGui::Render();

        // Width and height of the window
        int display_w, display_h;

        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Iterate over all sketches
        for (int i = 0; i < objectManager.getObjectCount(); i++)
        {
            if (i == objectManager.getSketchIndex())
            {
                    // Draw the selected sketch
                    objectManager.drawSelectedObject(io, width, height);
            }
        }

        // View all sketches
        objectManager.viewAllObject();


        // Iterate over all emission sketches
        for (int i = 0; i < emissionTextureManager.getTexturesCount(); i++)
        {
            if (i == emissionTextureManager.getTextureIndex())
            {
                // Draw the selected sketch
                emissionTextureManager.drawSelectedTextures(io, width, height, currentTime);
            }
        }

        // View all sketches
        emissionTextureManager.viewAllTextures(currentTime);

        glUseProgram(0);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Delete all VBO buffers
    objectManager.deleteAllVBOs();
    emissionTextureManager.deleteAllVBOs();

    // Cleanup
    cleanup(window);
    return 0;
}