#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Camera.hpp"
#include "Renderer.hpp"
#include "ProgressiveMarcher.hpp"
#include "Processing.hpp"
#include "ShaderUtils.hpp"


// global variables

// window dimensions
int WIDTH = 800, HEIGHT = 600;

// aspect ratio window, updated on resizing
float aspect = float(WIDTH) / HEIGHT;

// scalar field volume bounds + resolution
float minGrid = -5.0f;
float maxGrid = 5.0f;
float step = 0.2f;  // distance btwn sample point in grid

// core program stuff
Camera camera;                          // view direction and zoom
Renderer* renderer = nullptr;           // upload and draw mesh
ProgressiveMarcher* marcher = nullptr;  // generate tri. VIA marching cubes (PROGRESSIVLEY!!)

// mouse interaction
bool isDragging = false;
double lastX = 0.0, lastY = 0.0;    // last mouse coords

// shader program + locations
GLuint shaderID;
GLint loc_MVP, loc_V, loc_LightDir, loc_Color;

// callbacks

// window resizing, updates the viewpoint + recalculates the aspect ratio
void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
    aspect = float(w) / float(h);
}

// mouse button, enable camera dragging w/ mouse movement.
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            isDragging = true;
            glfwGetCursorPos(window, &lastX, &lastY);   // record the mouse position
        } else if (action == GLFW_RELEASE) {
            isDragging = false;                         // stop dragging
        }
    }
}

// mouse moved, if dragging then update the cam's angles
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!isDragging) return;

    float dx = static_cast<float>(xpos - lastX);
    float dy = static_cast<float>(ypos - lastY);

    camera.processMouseMotion(dx, dy);
    lastX = xpos;
    lastY = ypos;
}

// keyboard zoom and s key to save to PLY
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_UP) camera.processScroll(-0.2f);    // zoom in
        if (key == GLFW_KEY_DOWN) camera.processScroll(0.2f);   // zoom out
        if (key == GLFW_KEY_S) {                                // save to PLY
            // export the current mesh
            const auto& verts = marcher->getVertices();
            auto norms = compute_normals(verts);
            writePLY(verts, norms, "output.ply");
            std::cout << "PLY file saved as output.ply\n";
        }
    }
}

// two example scalar fields

// wave
float scalarField1(float x, float y, float z) {
    return y - sinf(x) * cosf(z);
}

// tunnel
float scalarField2(float x, float y, float z) {
    return x * x - y * y - z * z - z;
}

float scalarField3(float x, float y, float z){
    return x * x * x - y * y - z * z - z;
}

// main
int main() {
    // initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW.\n";
        return -1;
    }

    // make the window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Marching Cubes", nullptr, nullptr);

    // set up OpenGL context + load function w/ GLEW
    glfwMakeContextCurrent(window);
    glewInit();

    // register callback functions for user input
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetKeyCallback(window, key_callback);

    // enable buffering
    glEnable(GL_DEPTH_TEST);

    // Choose which scalar field to activate (initialize the marcher)
    marcher = new ProgressiveMarcher(scalarField1, 0.0f, minGrid, maxGrid, step);       // this is the wave
    // marcher = new ProgressiveMarcher(scalarField2, -1.5f, minGrid, maxGrid, step);   // this is the quadratic
    // marcher = new ProgressiveMarcher(scalarField3, -1.5f, minGrid, maxGrid, step);

    // renderer and zoom out the camera a bit (**might be an issue revisit this)
    renderer = new Renderer(minGrid, maxGrid);
    camera.processScroll(15.0f);

    // load and compile shaders
    shaderID = loadShaders("phong.vert", "phong.frag");

    // get the uniform locations from the shader program
    loc_MVP = glGetUniformLocation(shaderID, "MVP");
    loc_V = glGetUniformLocation(shaderID, "V");
    loc_LightDir = glGetUniformLocation(shaderID, "LightDir");
    loc_Color = glGetUniformLocation(shaderID, "modelColor");

    // main loop
    while (!glfwWindowShouldClose(window)) {
        // generate a portion of the mesh each frame (or else its painfully slow)
        bool updated = false;
        for (int i = 0; i < 100; ++i) {     // 100 steps
            if (!marcher->step()) break;    // stop if done
            updated = true;
        }
        if (updated) {
            // upload the newest mesh verti. + normals
            renderer->uploadMesh(marcher->getVertices(), marcher->getNormals());
        }

        // clear the screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);   // dark gray BG
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // cam + projection setup

        // camera
        glm::mat4 view = camera.getViewMatrix();

        // projection matrix
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

        // combined
        glm::mat4 mvp  = proj * view;

        // activate shader + set uniforms
        glUseProgram(shaderID);
        glUniformMatrix4fv(loc_MVP, 1, GL_FALSE, glm::value_ptr(mvp));
        glUniformMatrix4fv(loc_V, 1, GL_FALSE, glm::value_ptr(view));
        glUniform3f(loc_LightDir, 1.0f, 1.0f, 1.0f);    // the light
        glUniform3f(loc_Color, 0.8f, 0.3f, 0.2f);       // color of mesh (reddish pink)

        // render mesh + box
        renderer->draw(mvp, view);

        // display frame + poll
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // cleanup
    delete renderer;
    delete marcher;

    // exit
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
