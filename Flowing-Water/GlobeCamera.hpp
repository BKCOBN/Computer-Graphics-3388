// GlobeCamera.hpp
#ifndef GLOBE_CAMERA_HPP
#define GLOBE_CAMERA_HPP

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

float cameraTheta = 0.0f;   // horizontal (Y axis)
float cameraPhi = 1.57f;    // Vertical (up down)
float cameraRadius = 40.0f; // Distance from center point
float mouseSpeed = 0.005f;  // mouse sensitivity

// last mouse cursor position
double lastX = 0.0, lastY = 0.0;

// skip jump on first frame
bool firstMouse = true;

// camera controller function
// mods the view matrix based on mouse input and zoom lvl
void cameraControlsGlobe(glm::mat4 &ViewMatrix, float centerY) {
    // get the current camera position
    double xpos, ypos;
    glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);

    // initalize last position if this is first frame
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // compute change in mouse position
    float dx = xpos - lastX;
    float dy = ypos - lastY;

    // update stored mouse position
    lastX = xpos;
    lastY = ypos;

    // check if left mouse is being held down
    if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        // update angles
        cameraTheta += dx * mouseSpeed; // horizontal rotation
        cameraPhi -= dy * mouseSpeed;   // vertical rotation

        // clamp vert. angle to avoid flipping the cam upside down
        cameraPhi = glm::clamp(cameraPhi, 0.1f, 3.13f);
    }

    // convert spherical to cartesion position
    glm::vec3 eye(
        cameraRadius * sin(cameraPhi) * cos(cameraTheta), // X = r*sin(phi)*cos(theta)
        cameraRadius * cos(cameraPhi) + centerY,          // Y = r*cos(phi) + center offset
        cameraRadius * sin(cameraPhi) * sin(cameraTheta)  // Z = r*sin(phi)*sin(theta)
    );

    // look at origin
    glm::vec3 center(0.0f, centerY, 0.0f);

    // up direction
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    // build final view matrix
    ViewMatrix = glm::lookAt(eye, center, up);
}

#endif
