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
#include "GameLevel.hpp"
#include "NameComponent.hpp"
 
void onetap_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    InputHandler* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
    handler->addKeyEventToQ(key, action, mods);
}

void mouse_callback(GLFWwindow* window, double mouseX_, double mouseY_) {
    InputHandler* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
    handler->moveMouse(mouseX_, mouseY_);
}

void InputHandler::dumpTextToPlayer() {
    setHandlerMode(KCALL_DEFAULT);
    if (auto x = game->getPlayerHero()) {
        x->getComponent<NameComponent>()->speak(readText, 3.0);
    }
    readText = ""; 
}  

InputHandler::InputHandler() {// duplicate code, fix when more advanced callback state support is added
    callbackSets.insert(std::pair<int,CallbackSet>(KCALL_DEFAULT, CallbackSet()));
    callbackSets.insert(std::pair<int,CallbackSet>(KCALL_MENU, CallbackSet()));
    callbackSets.insert(std::pair<int,CallbackSet>(KCALL_READTEXT, CallbackSet()));
    mode = KCALL_DEFAULT;
    keyCallbacks = &(callbackSets[KCALL_DEFAULT]);
    mouseMovesCamera = true;
    readText = "";
}

InputHandler::~InputHandler() {
     
}

void InputHandler::setOneTapCallback(int i, keyCallback cbk) {
    if (keyCallbacks->oneTapCallbacks.find(i) == keyCallbacks->oneTapCallbacks.end()) {
        keyCallbacks->oneTapCallbacks.insert(std::pair<int, keyCallback>(i, cbk));
    } else { 
        keyCallbacks->oneTapCallbacks.find(i)->second = cbk;
    } 
} 

void InputHandler::addKeyEventToQ(int key, int action, int mods) {
    keyEventQ.push(KeyEvent(key, action, mods)); 
}

void InputHandler::setWindow(GLFWwindow *window_) {
    window = window_;
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, onetap_callback);
}

void InputHandler::tick() {
    while (!keyEventQ.empty()) {
        KeyEvent& ke = keyEventQ.front();
        keyEventQ.pop();
        if (ke.action == GLFW_PRESS) {
            currentKey = ke.key;
            notifyAll(KEY_PRESSED); 
             
            if (mode == KCALL_READTEXT && ke.key != GLFW_KEY_ENTER) {
                    const char* key_name = glfwGetKeyName(ke.key, 0);
                    if (!key_name) continue;
                    readText.append(key_name);
            } else {
                processInput(ke);
            }
        }  
    }
     
    for (auto i = keyCallbacks->continuousCallbacks.begin(); i != keyCallbacks->continuousCallbacks.end(); i++) {
        if (glfwGetKey(window, (*i).first) == GLFW_PRESS) {
            (*i).second(game);
        }
    }
}



void InputHandler::moveMouse(double mouseX_, double mouseY_) {
    if (firstMouse) {
        lastMX = mouseX_;
        lastMY = mouseY_;
        firstMouse = false;
    }
    if (mouseMovesCamera) {
        double xOffset = mouseX_ - lastMX;
        double yOffset = lastMY - mouseY_;
        activeCamera->rotate(glm::vec3(yOffset*0.03, xOffset*0.03, 0));
    } 

    lastMX = mouseX_;
    lastMY = mouseY_;
}


void InputHandler::swapCursorMode() {
    switch (glfwGetInputMode(window, GLFW_CURSOR)) {
        case GLFW_CURSOR_DISABLED:
            mouseMovesCamera = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
            
        case GLFW_CURSOR_NORMAL:
            mouseMovesCamera = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
    }
} 

int InputHandler::processInput(const KeyEvent& ke) {
    auto call = keyCallbacks->oneTapCallbacks.find(ke.key);
    if (ke.action == GLFW_PRESS && call != keyCallbacks->oneTapCallbacks.end()) {
        call->second(game);
        return 0;
    }
    return 0;
}


void InputHandler::setCamera(const std::shared_ptr<Camera>& cam) {
    activeCamera = cam;
}

void InputHandler::setGame(Game* game_) {
    game = game_;
}

GLenum InputHandler::getCurrentKeyPress() const {
    return currentKey;
}

void InputHandler::setHandlerMode(KeyCallbackSet set) {
    mode = set;
    keyCallbacks = &(callbackSets[set]);
}

void InputHandler::clear() { // duplicate code, fix when more advanced callback state support is added
    callbackSets.clear(); 
    callbackSets.insert(std::pair<int,CallbackSet>(KCALL_DEFAULT, CallbackSet()));
    callbackSets.insert(std::pair<int,CallbackSet>(KCALL_MENU, CallbackSet()));
    callbackSets.insert(std::pair<int,CallbackSet>(KCALL_READTEXT, CallbackSet()));
    mode = KCALL_DEFAULT;
    keyCallbacks = &(callbackSets[KCALL_DEFAULT]);
}

void InputHandler::setContinuousCallback(int i, keyCallback cbk) {
    if (keyCallbacks->continuousCallbacks.find(i) == keyCallbacks->continuousCallbacks.end()) {
        keyCallbacks->continuousCallbacks.insert(std::pair<int, keyCallback>(i, cbk));
    } else {
        keyCallbacks->continuousCallbacks.find(i)->second = cbk;
    }
} 
