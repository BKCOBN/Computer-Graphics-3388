#ifndef CAMERA_H
#define CAMERA_H
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


class Camera {
public:
    // camera position, direction that the camera is facing and the up vector
    glm::vec3 position, direction, up;
    float speed, rotationSpeed; // speed of movement for up and down arrows, and the spped of rotation for turning left or right

    // default constructor
    Camera();
    // all the functions
    void moveForward();
    void moveBackward();
    void rotateLeft();
    void rotateRight();
    glm::mat4 getViewMatrix();
};

#endif
