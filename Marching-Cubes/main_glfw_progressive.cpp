#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Camera.hpp"
#include "Renderer.hpp"
#include "ProgressiveMarcher.hpp"

Camera camera;
Renderer* renderer = nullptr;
ProgressiveMarcher* marcher = nullptr;

float minVal = -1.5f, maxVal = 1.5f, step = 0.1f;
int width = 800, height = 600;
float aspect = float(width) / float(height);

bool dragging = false;
double lastX = 0.0, lastY = 0.0;

float scalarField(float x, float y, float z) {
    return x*x + y*y + z*z - 1.0f;
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
    aspect = float(w) / float(h);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            dragging = true;
            glfwGetCursorPos(window, &lastX, &lastY);
        } else if (action == GLFW_RELEASE) {
            dragging = false;
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!dragging) return;
    float dx = static_cast<float>(xpos - lastX);
    float dy = static_cast<float>(ypos - lastY);
    camera.processMouseMotion(dx, dy);
    lastX = xpos;
    lastY = ypos;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_UP) camera.processScroll(-0.2f);
        if (key == GLFW_KEY_DOWN) camera.processScroll(0.2f);
    }
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    glm::mat4 mvp = proj * view;

    // Step marching cubes and update renderer
    if (marcher->step()) {
        renderer->uploadMesh(marcher->getVertices(), marcher->getNormals());
    }

    renderer->draw(mvp, view);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(width, height, "Marching Cubes (Progressive)", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewInit();

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetKeyCallback(window, key_callback);

    marcher = new ProgressiveMarcher(scalarField, 0.0f, minVal, maxVal, step);
    renderer = new Renderer(minVal, maxVal);

    while (!glfwWindowShouldClose(window)) {
        renderScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete renderer;
    delete marcher;
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
