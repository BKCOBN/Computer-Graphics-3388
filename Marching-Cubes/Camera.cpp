#include "Camera.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <algorithm>
#include <cmath>

// constructor
// initalize the cam (5, 5, 5) (spherical) looking at origin of (0,0,0)
Camera::Camera()
    : radius(8.66f),                // ~length of vector (5,5,5)
      theta(glm::radians(45.0f)),   // horizontal angle
      phi(glm::radians(45.0f))      // vertical angle
{
    updateViewMatrix(); // initial view matrix
}

// mouse interactions to let the user orbit cam around origin
void Camera::processMouseMotion(float deltaX, float deltaY) {
    const float sensitivity = 0.005f;   // just movement sensitivity

    // update horizontal and vertial roation
    theta -= deltaX * sensitivity;  // H
    phi   -= deltaY * sensitivity;  // V

    // clamp phi to avoid flipping over the top or bottom
    const float epsilon = 0.01f;
    phi = std::clamp(phi, epsilon, glm::pi<float>() - epsilon);

    // recalculate the cam veiew matrix
    updateViewMatrix();
}

// zoom in or out with arrow keys (scroll zoom)
void Camera::processScroll(float deltaRadius) {
    radius += deltaRadius;           // modify radius to zoom in or out
    radius = std::max(radius, 1.0f); // stop the camera from going inside the object (just a max)

    updateViewMatrix();
}

// manual update
void Camera::update() {
    updateViewMatrix();
}

// spherical coords -> cartesian and then generates a matrix pointing from cam to origin
void Camera::updateViewMatrix() {
    // conversion
    float x = radius * sin(phi) * cos(theta);
    float y = radius * cos(phi);
    float z = radius * sin(phi) * sin(theta);

    // new cam position
    position = glm::vec3(x, y, z);

    // make the matrix
    viewMatrix = glm::lookAt(position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

// return cam position
glm::vec3 Camera::getPosition() const {
    return position;
}

// return view matrix
glm::mat4 Camera::getViewMatrix() const {
    return viewMatrix;
}
