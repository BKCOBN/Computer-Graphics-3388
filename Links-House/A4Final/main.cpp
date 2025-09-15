#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.h"
#include "TexturedMesh.h"
#include "Shader.h"

// set up the camera object for movements
Camera camera;

// callback to handle keyboard input for camera movements
void keyCallback(GLFWwindow* window, int key, int, int action, int) {
    // if a key is pressed and held
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        // if up arrow key is held (forwards)
        if (key == GLFW_KEY_UP) camera.moveForward();
        // if down arrow key is held (backwards)
        else if (key == GLFW_KEY_DOWN) camera.moveBackward();
        // if left arrow key is held (rotate left on the spot)
        else if (key == GLFW_KEY_LEFT) camera.rotateLeft();
        // if right arrow key is held (rotate right on the spot)
        else if (key == GLFW_KEY_RIGHT) camera.rotateRight();
    }
}

// main execution
int main() {
    // initialize GLFW
    glfwInit();

    // make a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Link's House", NULL, NULL);

    // opengl context current
    glfwMakeContextCurrent(window);

    // initaize GLEW
    glewInit();

    // set up key callback
    glfwSetKeyCallback(window, keyCallback);

    // load shaders
    Shader shader("vertex_shader.glsl", "fragment_shader.glsl");

    // load all TexturedMesh objects
    TexturedMesh bottles("Bottles.ply", "bottles.bmp", &shader);
    TexturedMesh curtains("Curtains.ply", "curtains.bmp", &shader);
    TexturedMesh floor("Floor.ply", "floor.bmp", &shader);
    TexturedMesh table("Table.ply", "table.bmp", &shader);
    TexturedMesh walls("Walls.ply", "walls.bmp", &shader);
    TexturedMesh windowbg("WindowBG.ply", "windowbg.bmp", &shader);
    TexturedMesh woodobjects("WoodObjects.ply", "woodobjects.bmp", &shader);
    TexturedMesh metalObjects("MetalObjects.ply", "metalobjects.bmp", &shader);
    TexturedMesh doorbg("DoorBG.ply", "doorbg.bmp", &shader);
    TexturedMesh patio("Patio.ply", "patio.bmp", &shader);


    // set projection matrix 45° perspective view
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // enable depth buffering so the objects load in the correct way
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // rendering loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear screen 4 next frame
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 MVP = projection * view;

        // draw every textured mesh
        bottles.draw(MVP);
        curtains.draw(MVP);
        floor.draw(MVP);
        table.draw(MVP);
        walls.draw(MVP);
        windowbg.draw(MVP);
        woodobjects.draw(MVP);
        // have the transparent objects render last
        metalObjects.draw(MVP);
        doorbg.draw(MVP);
        patio.draw(MVP);


        // swap to display frame + poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup b4 exit
    glfwDestroyWindow(window);
    glfwTerminate();
}
