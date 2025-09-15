#include "Camera.h"
#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// camera constructor
Camera::Camera() {
    position = glm::vec3(0.5f, 0.4f, 0.5f);     // initial position
    direction = glm::vec3(0.0f, 0.0f, -1.0f);   // initial direction vector (-'ve Z axis)
    up = glm::vec3(0.0f, 1.0f, 0.0f);           // define 'up' vector (Y axis)
    speed = 0.05f;                              // camera speed (small to keep it smooth)
    rotationSpeed = glm::radians(3.0f);         // rotation speed (3 degrees to radians)
}

// move the camera forwards (in the direction it is facing)(up arrow)
void Camera::moveForward() {
    position += speed * direction;
}

// move the camera backwards {in the ooposite direction that it is facing)(down arrow)
void Camera::moveBackward() {
    position -= speed * direction;
}

// rotate the camera to the left on the spot
void Camera::rotateLeft() {
    // poitive rotation speed angle (compute the sin and cos to apply the transformation)
    float angle = rotationSpeed, cosA = cos(angle), sinA = sin(angle);

    // apply rotation to the camera's direction vector
    // rotates around the Y axis (UP) (so we don't change that)
    // new x = cos(angle)*oldX - sin(angle)*oldZ
    // new z is the same but sin and cos switched and added (sinX + cosZ)
    direction = glm::normalize(glm::vec3(cosA * direction.x - sinA * direction.z, direction.y, sinA * direction.x + cosA * direction.z));
}

// rotate the canera to the right on the spot
void Camera::rotateRight() {
    // negative of the rotation speed
    float angle = -rotationSpeed, cosA = cos(angle), sinA = sin(angle);

    // apply the rotation, not writing that stuff out again
    direction = glm::normalize(glm::vec3(cosA * direction.x - sinA * direction.z, direction.y, sinA * direction.x + cosA * direction.z));
}

// get the view matrix for the camera
glm::mat4 Camera::getViewMatrix() {
    // world coords -> view (camera) coords
    return glm::lookAt(position, position + direction, up);
}
