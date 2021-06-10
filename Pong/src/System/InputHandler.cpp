//
//  InputHandler.cpp
//  Pong
//
//  Created by Joseph Gu on 6/3/21.
//

#include "InputHandler.hpp"
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include "Game.hpp"


void onetap_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    game->processInput(key, action, mods);
}

void onetap_callback0(GLFWwindow* window, int key, int scancode, int action, int mods) {
}

void char_callback(GLFWwindow* window, unsigned int key) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    //game->processInput2(key, action);
}

void mouse_callback(GLFWwindow* window, double mouseX_, double mouseY_) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    game->moveMouse(mouseX_, mouseY_);
}

InputHandler::InputHandler() {

}
InputHandler::~InputHandler() {
    
}

void InputHandler::setWindow(GLFWwindow *window_) {
    window = window_;
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, onetap_callback);
}

void InputHandler::setKeyCallback(GLFWkeyfun func) {
    glfwSetKeyCallback(window, func);
}

void InputHandler::setCharCallback(GLFWcharfun func) {
    glfwSetCharCallback(window, func);
}

void InputHandler::tick() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
