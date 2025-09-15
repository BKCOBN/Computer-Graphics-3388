#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLFWwindow* window;

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

#include "PlaneMesh.hpp"
#include "GlobeCamera.hpp"


int main(int argc, char* argv[]) {
    // settings
    float screenW = 1500;   // window width
    float screenH = 1500;   // window height
    float stepsize = 1.0f;  // distance btw verts in the grid
    float xmin = -10;       // left bounds
    float xmax = 10;        // right bounds

    // override defaults w/ command line args
    if (argc > 1 ) {
		screenW = atoi(argv[1]);
	}
	if (argc > 2) {
		screenH = atoi(argv[2]);
	}
	if (argc > 3) {
		stepsize = atof(argv[3]);
	}
	if (argc > 4) {
		xmin = atof(argv[4]);
	}
	if (argc > 5) {
		xmax = atof(argv[5]);
	}

    // Initialise GLFW
    if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

    // antialiasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(screenW, screenH, "Water Shader", NULL, NULL);
    if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

    // Create the mesh given stepsize
    PlaneMesh plane(xmin, xmax, stepsize);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.2f, 0.2f, 0.3f, 0.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Camera and projection
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), screenW / screenH, 0.001f, 1000.0f);
    glm::mat4 V;
    glm::vec3 lightpos(5.0f, 30.0f, 5.0f);
    glm::vec4 color1(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    do {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Zoom control
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            cameraRadius -= 0.5f;
            if (cameraRadius < 1.0f) cameraRadius = 1.0f; // Prevent going inside the mesh
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            cameraRadius += 0.5f;
            if (cameraRadius > 200.0f) cameraRadius = 200.0f; // Clamp far zoom
        }

        cameraControlsGlobe(V, 5);

        int currentMode = 0;
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) currentMode = 0;
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) currentMode = 1;
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) currentMode = 2;
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) currentMode = 3;

        plane.setDebugMode(currentMode);

        plane.draw(lightpos, V, Projection);

        // Swap buggers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}
