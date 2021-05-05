
//  main.cpp
//  Pong
//
//  Created by Joseph Gu on 4/24/21.
//

#include <GL/glew.h>
#define GLFW_DLL
#include "World.hpp"
#include "Renderer.hpp"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(600, 600, "OpenGL", NULL, NULL); // Windowed
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    glewInit();
    
    World world;
    cout << &world << "\n";
    Renderer renderer(&world, window);
    renderer.loadVertices();
    
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS);
    
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer.render();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
}
