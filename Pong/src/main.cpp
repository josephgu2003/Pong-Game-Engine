
//  main.cpp
//  Pong
//
//  Created by Joseph Gu on 4/24/21.
//

#include <GL/glew.h>
#define GLFW_DLL
#include "World.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

using namespace std;

double lastMX = 500;
double lastMY = 400;
double xOffset;
double yOffset;
bool firstMouse = true;

PlayerHero* pHero = new PlayerHero();
Camera* camera = new Camera();

void onetap_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        pHero -> jump();
    }
}

void mouse_callback(GLFWwindow* window, double mouseX_, double mouseY_) {
    if (firstMouse) {
        lastMX = mouseX_;
        lastMY = mouseY_;
        firstMouse = false;
    }
    xOffset = mouseX_ - lastMX;
    yOffset = lastMY - mouseY_;
    lastMX = mouseX_;
    lastMY = mouseY_;
    camera->incYaw(xOffset*0.03);
    camera->incPitch(yOffset*0.03);
    xOffset = 0;
    yOffset = 0;
}

int main() {
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(1000, 800, "OpenGL", NULL, NULL); // Windowed
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    glewInit();
    
    camera->setActor(pHero);
    pHero->loadModel();
    World* world = new World;
    world->insertActor(pHero);
    Renderer* renderer = new Renderer;
    renderer->setWorld(world);
    renderer->setCamera(camera);
    renderer->loadActorData();
    renderer->loadMapData();
    renderer->loadSkyBoxData();
    
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LEQUAL);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    glfwSetKeyCallback(window, onetap_callback);
    glfwSetTime(0);
    
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
            std::cout << camera->getYaw() << camera->getPitch() << camera->getRoll() << "\n";
            std::cout << camera->dirVec.x << camera->dirVec.y <<camera->dirVec.z << "\n";
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            pHero -> posDir(0.04);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            pHero -> posDir(-0.04);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            pHero -> posRight(0.04);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            pHero -> posRight(-0.04);
        }
        renderer->render();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
}
