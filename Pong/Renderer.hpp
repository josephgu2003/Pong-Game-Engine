//
//  Renderer.hpp
//  Pong
//
//  Created by Joseph Gu on 5/3/21.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "Shader.hpp"
#include "World.hpp"
#include "Camera.hpp"

void mouse_callback(GLFWwindow* window, double mouseX, double mouseY);

class Renderer {
    World* world;
    Shader* shader;
    glm::mat4 modelMat;
    glm::mat4 viewMat;
    glm::mat4 projMat;
    GLFWwindow* window;
    Camera camera;
public:
    Renderer(World *world, GLFWwindow* window_);
    ~Renderer();
    void loadVertices();
    void render();
    void findMatrices(int n); // finds the matrices for nth actor in world's actor vector
};
#endif /* Renderer_hpp */
