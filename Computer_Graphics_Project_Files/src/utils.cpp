// utils.cpp

#include "utils.h"
#include "objectManager.h"
#include "emissionTextureManager.h"
#include "graphManager.h"
#include "node.h"
#include "edge.h"
#include "sketching.h"
#include "emissionTexture.h"
# include "emissionProperties.h"

#include <iostream>

extern ImVec4 selectedColor;

extern bool sketchMode;
extern bool dynamicSketchMode;
extern bool editMode;
extern bool animMode;
extern bool graphMode;

bool anim_loop = false;
float anim_dur = 1.0f;
float to_dx = 0.0f, to_dy = 0.0f, to_sx = 1.0f, to_sy = 1.0f, to_angle;
float px = 0.0f, py = 0.0f;

float _r, _s;
int _no1, _no2, _rel;

void cleanup(GLFWwindow* window)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}


void showOptionsDialog(ImGuiIO &io, ObjectManager& objectManager, EmissionTextureManager& emissionTextureManager, GraphManager& graphManager, GLuint shaderProgram, GLint selectedColor_uniform)
{
    // Create a tab bar
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
    ImGui::SetNextWindowPos(ImVec2(0, 0));  // Set the window position to the top-left corner
    ImGui::Begin("Toolbox", nullptr, window_flags);

    // Create a tab bar
    if (ImGui::BeginTabBar("Modes"))
    {
        // Tab for "Object Mode"
        if (ImGui::BeginTabItem("Object Mode"))
        {
            sketchMode = true;

            if(sketchMode)
            {
                dynamicSketchMode = false;
                editMode = false;
                animMode = false;
                graphMode = false;

                if (ImGui::Button("Add Sketches"))
                {
                    objectManager.createAndAddSketch();
                    Node* sketchNode = new Node(1, objectManager.getObjects()[objectManager.getSketchIndex()], nullptr);
                    graphManager.AddSketchToList(sketchNode);
                    graphManager.nodeCount++;
                }

                // Display a list of added sketches with automatically generated names
                for (int i = 0; i < objectManager.getObjectCount(); i++)
                {
                    if (ImGui::Selectable(("Sketch " + std::to_string(objectManager.getObjects()[i]->getId())).c_str(), i == objectManager.getSketchIndex()))
                    {
                        objectManager.selectObject(i);
                    }
                }

                if (objectManager.getSketchIndex() != -1)
                {
                    if(ImGui::Button("Delete Selected Sketch"))
                    {
                        // Handle the action for deleting the selected sketch
                        objectManager.deleteSelectedSketch();

                        // After deleting the sketch, update the selected index and ensure the list is in sync
                        if (objectManager.getSketchIndex() >= objectManager.getObjectCount())
                        {
                            // If the selected index is out of bounds, set it to a valid value
                            objectManager.selectObject(objectManager.getObjectCount() - 1);
                        }
                    }
                }
            }
            else
            {
                ImGui::Text("sketching mode not enabled");
            }

            ImGui::EndTabItem();
        }

        // Tab for "Dynamic Object Mode"
        if (ImGui::BeginTabItem("Dynamic Object Mode"))
        {
            dynamicSketchMode = true;

            if(dynamicSketchMode)
            {
                sketchMode = false;
                editMode = false;
                animMode = false;
                graphMode = false;

                if (ImGui::Button("Add Emission Texture"))
                {
                    emissionTextureManager.createAndAddTextures();
                    Node* emissionNode = new Node(2, nullptr, emissionTextureManager.getTextures()[emissionTextureManager.getTextureIndex()]);
                    graphManager.AddSketchToList(emissionNode);
                    graphManager.nodeCount++;
                }

                // Display a list of added dynamic sketches with automatically generated names
                for (int i = 0; i < emissionTextureManager.getTexturesCount(); i++)
                {
                    if (ImGui::Selectable(("Emission " + std::to_string(emissionTextureManager.getTextures()[i]->getId())).c_str(), i == emissionTextureManager.getTextureIndex()))
                    {
                        emissionTextureManager.selectTextures(i);
                    }
                }

                if (emissionTextureManager.getTextureIndex() != -1)
                {
                    int c_level = emissionTextureManager.getTextures()[emissionTextureManager.getTextureIndex()]->getCreationLevel();
                    if(c_level < 5)
                    {
                        if(c_level == 0){ImGui::Text("Draw the emission particle");}
                        else if(c_level == 1){ImGui::Text("Draw the emission mask");}
                        else if(c_level == 2){ImGui::Text("Draw the emission surface");}
                        else if(c_level == 3){ImGui::Text("Draw the emission path");}

                        else if(c_level == 4)
                        {
                            ImGui::Text("Set rate and speed");

                            ImGui::InputFloat("Rate ", &_r);
                            ImGui::InputFloat("Speed ", &_s);

                            emissionTextureManager.getTextures()[emissionTextureManager.getTextureIndex()]->setRate(_r);
                            emissionTextureManager.getTextures()[emissionTextureManager.getTextureIndex()]->setSpeed(_s);
                        }

                        if (ImGui::Button("Go to next Step"))
                        {
                            c_level += 1;
                            emissionTextureManager.getTextures()[emissionTextureManager.getTextureIndex()]->setCreationLevel(c_level);
                        }
                    }
                    else{ImGui::Text("Dynamic object created");}

                    if(ImGui::Button("Delete Selected Sketch"))
                    {
                        // Handle the action for deleting the selected sketch
                        emissionTextureManager.deleteSelectedTextures();

                        // After deleting the sketch, update the selected index and ensure the list is in sync
                        if (emissionTextureManager.getTextureIndex() >= emissionTextureManager.getTexturesCount())
                        {
                            // If the selected index is out of bounds, set it to a valid value
                            emissionTextureManager.selectTextures(emissionTextureManager.getTexturesCount() - 1);
                        }
                    }
                }
            }
            else
            {
                ImGui::Text("dynamic sketching mode not enabled");
            }

            ImGui::EndTabItem();
        }

        // Tab for "Edit Mode"
        if (ImGui::BeginTabItem("Edit Mode"))
        {
            editMode = true;

            // Display a list of added sketches with automatically generated names
            for (int i = 0; i < objectManager.getObjectCount(); i++)
            {
                if (ImGui::Selectable(("Sketch " + std::to_string(objectManager.getObjects()[i]->getId())).c_str(), i == objectManager.getSketchIndex()))
                {
                    objectManager.selectObject(i);
                    emissionTextureManager.setTextureIndex(-1);
                }
            }

            // Display a list of added dynamic sketches with automatically generated names
            for (int i = 0; i < emissionTextureManager.getTexturesCount(); i++)
            {
                if (ImGui::Selectable(("Emission " + std::to_string(emissionTextureManager.getTextures()[i]->getId())).c_str(), i == emissionTextureManager.getTextureIndex()))
                {
                    emissionTextureManager.selectTextures(i);
                    objectManager.setSketchIndex(-1);
                }
            }

            if (ImGui::ColorEdit4("Pick Line Color", &selectedColor.x))
            {
                glUseProgram(shaderProgram); // Activate the shader program
                glUniform4f(selectedColor_uniform, selectedColor.x, selectedColor.y, selectedColor.z, selectedColor.w);
                glUseProgram(0);
            }

            // Apply the changes to the selected sketch
            if(editMode && objectManager.getSketchIndex() != -1)
            {
                ImGui::InputFloat("Translation X", &to_dx);
                ImGui::InputFloat("Translation Y", &to_dy);
                ImGui::InputFloat("Rotation", &to_angle);
                ImGui::InputFloat("Pivot X", &px);
                ImGui::InputFloat("Pivot Y", &py);
                ImGui::InputFloat("Scale X", &to_sx);
                ImGui::InputFloat("Scale Y", &to_sy);

                sketchMode = false;
                dynamicSketchMode = false;
                animMode = false;
                graphMode = false;

                if (ImGui::Button("Apply"))
                {
                    Sketching* selectedSketch = objectManager.getObjects()[objectManager.getSketchIndex()];
                    if (selectedSketch != nullptr && editMode)
                    {
                        glm::mat3 editMat = selectedSketch->getEditTransformationMatrix();

                        editMat = selectedSketch->move(editMat, to_dx, to_dy);
                        editMat = selectedSketch->rotate(editMat, to_angle, px, py);
                        editMat = selectedSketch->scale(editMat, to_sx, to_sy);

                        selectedSketch->editDrawing(editMat);

                        // Reset values to zero and 1 after applying changes
                        to_dx = 0.0f;
                        to_dy = 0.0f;
                        to_angle = 0.0f;
                        px = 0.0f;
                        py = 0.0f;
                        to_sx = 1.0f;
                        to_sy = 1.0f;
                    }
                }
            }
            // Apply the changes to the selected sketch
            if(editMode && emissionTextureManager.getTextureIndex() != -1)
            {
                sketchMode = false;
                dynamicSketchMode = false;
                animMode = false;
                graphMode = false;

                ImGui::InputFloat("Rate ", &_r);
                ImGui::InputFloat("Speed ", &_s);

                emissionTextureManager.getTextures()[emissionTextureManager.getTextureIndex()]->setRate(_r);
                emissionTextureManager.getTextures()[emissionTextureManager.getTextureIndex()]->setSpeed(_s);
            }
            else if(editMode && objectManager.getSketchIndex() == -1 && emissionTextureManager.getTextureIndex() == -1)
            {
                ImGui::Text("Nothing selected");
            }

            ImGui::EndTabItem();
        }

        // Tab for "Animation Mode"
        if (ImGui::BeginTabItem("Animation Mode"))
        {
            animMode = true;

            // Display a list of added sketches with automatically generated names
            for (int i = 0; i < objectManager.getObjectCount(); i++)
            {
                if (ImGui::Selectable(("Sketch " + std::to_string(objectManager.getObjects()[i]->getId())).c_str(), i == objectManager.getSketchIndex()))
                {
                    objectManager.selectObject(i);
                }
            }

            if(animMode && objectManager.getSketchIndex() != -1)
            {
                sketchMode = false;
                dynamicSketchMode = false;
                editMode = false;
                graphMode = false;

                Sketching* selectedSketch = objectManager.getObjects()[objectManager.getSketchIndex()];

                ImGui::Checkbox("Enable Looping", &anim_loop);
                ImGui::InputFloat("Animation Duration", &anim_dur);
                ImGui::InputFloat("To Translation X", &to_dx);
                ImGui::InputFloat("To Translation Y", &to_dy);
                ImGui::InputFloat("To Rotation", &to_angle);
                ImGui::InputFloat("Pivot X", &px);
                ImGui::InputFloat("Pivot Y", &py);

                if (ImGui::Button("Add & Save Animation"))
                {
                    selectedSketch->createAndAddAnimation();
                }

                // Display a list of added animations with automatically generated names
                for (int i = 0; i < selectedSketch->getAnimCount(); i++)
                {
                    if (ImGui::Selectable(("Animation " + std::to_string(selectedSketch->getAnimatingList()[i]->getId())).c_str(), i == selectedSketch->getSelectedAnimIndex()))
                    {
                        selectedSketch->selectAnimation(i);
                    }
                }

                if(selectedSketch->getSelectedAnimIndex() != -1)
                {
                    Animating* selectedAnimation = selectedSketch->getAnimatingList()[selectedSketch->getSelectedAnimIndex()];

                    if(!selectedAnimation->getDecided())
                    {
                        selectedAnimation->SetInitialAnimParameters(anim_loop,
                                                    anim_dur,
                                                    to_dx,
                                                    to_dy,
                                                    to_angle,
                                                    px,
                                                    py);

                        // Reset values to zero and 1 after applying changes
                        anim_loop = false;
                        anim_dur = 1.0f;
                        to_dx = 0.0f;
                        to_dy = 0.0f;
                        to_angle = 0.0f;
                        px = 0.0f;
                        py = 0.0f;
                    }

                    glm::mat3 animMatrix = selectedAnimation->getAnimTransformationMatrix();

                    if (ImGui::Button("Play Animation"))
                    {
                        selectedSketch->resetAnimation(selectedAnimation,
                                                       selectedAnimation->getAnimTransformationMatrix(),
                                                       selectedAnimation->getAnimationTime(),
                                                       selectedAnimation->getAnimationDuration(),
                                                       selectedAnimation->to_dx,
                                                       selectedAnimation->to_dy,
                                                       selectedAnimation->to_angle,
                                                       selectedAnimation->px,
                                                       selectedAnimation->py);

                        selectedAnimation->PlayAnimation(selectedAnimation->getIsLooping());

                        // Display a list of all edges
                        for (int i = 0; i < graphManager.edgeCount; i++)
                        {
                            // for sketch to sketch relation
                            if(graphManager.GetEdgeList()[graphManager.selectedEdgeIndex]->Node1->NodeType1 == selectedSketch)
                            {
                                std::cout<<"activate edge connection"<<std::endl;
                            }
                        }
                    }

                    if (ImGui::Button("Pause Animation"))
                    {
                        selectedAnimation->PauseAnimation();
                    }

                    if (ImGui::Button("Reset Animation"))
                    {
                        selectedSketch->resetAnimation(selectedAnimation,
                                                       selectedAnimation->getAnimTransformationMatrix(),
                                                       selectedAnimation->getAnimationTime(),
                                                       selectedAnimation->getAnimationDuration(),
                                                       selectedAnimation->to_dx,
                                                       selectedAnimation->to_dy,
                                                       selectedAnimation->to_angle,
                                                       selectedAnimation->px,
                                                       selectedAnimation->py);
                    }

                    if(selectedAnimation->getIsPlaying())
                    {
                        selectedSketch->animateObject(selectedAnimation ,
                                                      animMatrix,
                                                      selectedAnimation->to_dx,
                                                      selectedAnimation->to_dy,
                                                      selectedAnimation->to_angle,
                                                      selectedAnimation->px,
                                                      selectedAnimation->py);
                    }

                    if(ImGui::Button("Delete Selected Animation"))
                    {
                        // Handle the action for deleting the selected animation
                        selectedSketch->deleteSelectedAnim();

                        // After deleting the sketch, update the selected index and ensure the list is in sync
                        if (selectedSketch->getSelectedAnimIndex() >= selectedSketch->getAnimCount())
                        {
                            // If the selected index is out of bounds, set it to a valid value
                            selectedSketch->selectAnimation(selectedSketch->getAnimCount() - 1);
                        }
                    }
                }
            }
            else if(animMode && objectManager.getSketchIndex() == -1)
            {
                ImGui::Text("No object selected");
            }

            ImGui::EndTabItem();
        }

        // Tab for "Graph Mode"
        if (ImGui::BeginTabItem("Graph Mode"))
        {
            graphMode = true;

            if(graphMode)
            {
                sketchMode = false;
                dynamicSketchMode = false;
                editMode = false;
                animMode = false;

                if (ImGui::Button("Add Edges"))
                {
                    graphManager.createAndAddEdge();
                }

                // Display a list of all edges
                for (int i = 0; i < graphManager.edgeCount; i++)
                {
                    if (ImGui::Selectable(("Edge " + std::to_string(graphManager.GetEdgeList()[i]->getId())).c_str(), i == graphManager.selectedEdgeIndex))
                    {
                        graphManager.selectedEdgeIndex = i;
                    }
                }

                if (graphManager.selectedEdgeIndex != -1)
                {
                    int c_level = graphManager.GetEdgeList()[graphManager.selectedEdgeIndex]->getCreationLevel();
                    if(c_level < 3)
                    {
                        if(c_level == 0)
                        {
                            ImGui::Text("Select first node");

                            // Display a list of added nodes
                            int l = 0,m =0;
                            for (int i = 0; i < graphManager.GetNodeList().size(); i++)
                            {
                                if(graphManager.GetNodeList()[i]->nodeType == 1)
                                {
                                    ImGui::Text("Node Sketch %d", objectManager.getObjects()[l]->getId());
                                    l += 1;
                                }
                                else if(graphManager.GetNodeList()[i]->nodeType == 2)
                                {
                                    ImGui::Text("Node Emitter %d", emissionTextureManager.getTextures()[m]->getId());
                                    m += 1;
                                }
                            }

                            ImGui::InputInt("Node 1 ", &_no1);
                            graphManager.GetEdgeList()[graphManager.selectedEdgeIndex]->setNode1(graphManager.GetNodeList()[_no1]);
                        }

                        else if(c_level == 1)
                        {
                            ImGui::Text("Select second node");

                            // Display a list of added nodes
                            int l = 0,m =0;
                            for (int i = 0; i < graphManager.GetNodeList().size(); i++)
                            {
                                if(graphManager.GetNodeList()[i]->nodeType == 1)
                                {
                                    ImGui::Text("Node Sketch %d", objectManager.getObjects()[l]->getId());
                                    l += 1;
                                }
                                else if(graphManager.GetNodeList()[i]->nodeType == 2)
                                {
                                    ImGui::Text("Node Emitter %d", emissionTextureManager.getTextures()[m]->getId());
                                    m += 1;
                                }
                            }

                            ImGui::InputInt("Node 1 ", &_no2);
                            graphManager.GetEdgeList()[graphManager.selectedEdgeIndex]->setNode2(graphManager.GetNodeList()[_no2]);
                        }

                        else if(c_level == 2)
                        {
                            ImGui::Text("select relation (1=linear, 2=square, 3=cubic)");
                            ImGui::InputInt("Relation ", &_rel);
                            graphManager.GetEdgeList()[graphManager.selectedEdgeIndex]->setRelation(_rel);
                        }

                        if (ImGui::Button("Go to next Step"))
                        {
                            c_level += 1;
                            graphManager.GetEdgeList()[graphManager.selectedEdgeIndex]->setCreationLevel(c_level);
                        }
                    }
                    else if (c_level>=3 && (_no1 == _no2))
                    {
                        ImGui::Text("Incorrect Edge");

                        // Handle the action for deleting the selected sketch
                        graphManager.deleteSelectedEdges();

                        // After deleting the sketch, update the selected index and ensure the list is in sync
                        if (graphManager.selectedEdgeIndex >= graphManager.GetEdgeList().size())
                        {
                            // If the selected index is out of bounds, set it to a valid value
                            graphManager.selectedEdgeIndex = - 1;
                        }
                    }
                    else{ImGui::Text("Edge created");}

                    if(ImGui::Button("Delete Selected Edge"))
                    {
                        // Handle the action for deleting the selected sketch
                        graphManager.deleteSelectedEdges();

                        // After deleting the sketch, update the selected index and ensure the list is in sync
                        if (graphManager.selectedEdgeIndex >= graphManager.GetEdgeList().size())
                        {
                            // If the selected index is out of bounds, set it to a valid value
                            graphManager.selectedEdgeIndex = - 1;
                        }
                    }
                }
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::End();
}

void setVAO(unsigned int &VAO)
{
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

GLuint createShader(const char* filename, GLenum type)
{
    const GLchar* source = getShaderCode(filename);
    if (source == NULL) {
        fprintf(stderr, "Error opening %s: ", filename); perror("");
        return 0;
    }
    GLuint res = glCreateShader(type);
    glShaderSource(res, 1, &source, NULL);
    free((void*)source);

    glCompileShader(res);
    GLint compile_ok = GL_FALSE;
    glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
    if (compile_ok == GL_FALSE) {
        // fprintf(stderr, "%s:", filename);
        std::cout << "Error in compilation of :" << filename << std::endl;
        glDeleteShader(res);
        return 0;
    }

    return res;
}

const char * setGLSLVersion(){
#if __APPLE__
    // GL 3.2 + GLSL 150
        const char* glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    return glsl_version;
}

int openGLInit(){
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
    bool err = gladLoadGL(glfwGetProcAddress) == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
    bool err = false;
    glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
    bool err = false;
    glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
#else
    bool err = false;
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        exit(1);
    }

    //Enable multisampling
    glEnable(GL_MULTISAMPLE);

    // Enable Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set point size
    glPointSize(10.0f);
    glEnable(GL_POINT_SMOOTH);

    // Enable smooth point rendering
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    return 1;
}

unsigned int createProgram(const char *vshader_filename, const char* fshader_filename)
{
    //Create shader objects
    GLuint vs, fs;
    if ((vs = createShader(vshader_filename, GL_VERTEX_SHADER))   == 0) return 0;
    if ((fs = createShader(fshader_filename, GL_FRAGMENT_SHADER)) == 0) return 0;

    //Creare program object and link shader objects
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    GLint link_ok;
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        // fprintf(stderr, "glLinkProgram error:");
        // printLog(program);
        std::cout << "Linking error " << std::endl  ;
        glDeleteShader(vs);
        glDeleteShader(fs);
        glDeleteProgram(program);
        return 0;
    }
    return program;
}


char * getShaderCode(const char* filename)
{
    FILE* input = fopen(filename, "rb");
    if(input == NULL) return NULL;

    if(fseek(input, 0, SEEK_END) == -1) return NULL;
    long size = ftell(input);
    if(size == -1) return NULL;
    if(fseek(input, 0, SEEK_SET) == -1) return NULL;

    /*if using c-compiler: dont cast malloc's return value*/
    char *content = (char*) malloc( (size_t) size +1  );
    if(content == NULL) return NULL;

    fread(content, 1, (size_t)size, input);
    if(ferror(input)) {
        free(content);
        return NULL;
    }

    fclose(input);
    content[size] = '\0';
    return content;
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* setupWindow(int width, int height)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        exit(0);

    // Decide GL+GLSL versions
    const char * glsl_version = setGLSLVersion();


    // Create window with graphics context
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window = glfwCreateWindow(width, height, "COMPUTER GRAPHICS PROJECT", NULL, NULL);
    if (window == NULL)
        exit(0);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
    int status = openGLInit();
    if(!status){
        std::cout << "Initialized OpenGL succesfully " << std::endl;
    }
    std::cout<< "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return window;
}