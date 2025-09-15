#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/glm.hpp"

class Camera {
public:
    // initialize camera angles and compute view matrix
    Camera();

    // update camera orbit based on mouse movement
    void processMouseMotion(float deltaX, float deltaY);

    // adjust radius (zoom) based on scroll input
    void processScroll(float deltaRadius);

    // manual update for view matrix
    void update();

    // current position of camera and view matrix
    glm::vec3 getPosition() const;
    glm::mat4 getViewMatrix() const;

private:
    float radius;   // distance from origin
    float theta;    // horizontal roation angle (rad)
    float phi;      // vertical rotation angle (rad)

    glm::vec3 position;     // position of camera
    glm::mat4 viewMatrix;   // view matrix

    void updateViewMatrix(); // recalculate position + view matrix based on spherical coords
};

#endif
