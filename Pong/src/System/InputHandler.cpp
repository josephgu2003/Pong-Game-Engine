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
#include "Speech.hpp"
#include "FallingLetters.hpp"
#include "Fish.hpp"
#include "CombatComponent.hpp"

void onetap_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    InputHandler* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
    handler->processInput(key, action, mods);
}

void mouse_callback(GLFWwindow* window, double mouseX_, double mouseY_) {
    InputHandler* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
    handler->moveMouse(mouseX_, mouseY_);
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

void InputHandler::tick() {
    

    if (scheme == 0) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        activeHero->posDir(0.03);

    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        activeHero->posDir(-0.03);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        activeHero->posRight(0.03);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        activeHero->posRight(-0.03);
    }

    }
     
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    double mx, my;
    
    if (scheme == 2)     {
        glfwGetCursorPos(window, &mx, &my);
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

            }
    lastMY = my;
    lastMX = mx;
    }
}

void InputHandler::setActionScheme(int id) {
    if(id == 2) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(window, mouse_callback);
    }
    if(scheme == 2 && id !=2) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    scheme = id;
}

void InputHandler::moveMouse(double mouseX_, double mouseY_) {
    if (scheme != 2) {
    if (firstMouse) {
        lastMX = mouseX_;
        lastMY = mouseY_;
        firstMouse = false;
    }
    xOffset = mouseX_ - lastMX;
    yOffset = lastMY - mouseY_;
    lastMX = mouseX_;
    lastMY = mouseY_;
    activeCamera->incYaw(xOffset*0.03);
    activeCamera->incPitch(yOffset*0.03);
    xOffset = 0;
    yOffset = 0;
    }
}

int InputHandler::processInput(int key, int action, int mods) {
//    unique_lock<mutex> lock()
    if (scheme == 3) {
        int nextBranch;
        if (key == GLFW_KEY_A && action == GLFW_PRESS) {
            nextBranch = 0;
            if(activeDialogue != NULL) activeDialogue->branch(nextBranch);
            setActionScheme(0);
        }
            if (key == GLFW_KEY_B && action == GLFW_PRESS) {
                nextBranch = 1;
                if(activeDialogue != NULL) activeDialogue->branch(nextBranch);
                setActionScheme(0);
            }
    }
    if (scheme == 1) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_SPACE)
        {
          //  input.put(' ');
            i.append(" ");
        }
    const char* key_name = glfwGetKeyName((key), 0);
    if (key_name != NULL)
       // input << (key_name);
        if (mods == GLFW_MOD_SHIFT){
        char c = toupper(*key_name);
            if (key == GLFW_KEY_1) c = '!';
            if (key == GLFW_KEY_SLASH) c = '?';
            if (key == GLFW_KEY_APOSTROPHE) c = '\"';
        i.append(std::string(1, c));
         } else if (mods != GLFW_MOD_SHIFT) {
            i.append(key_name);
         }
    }
        
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
            scheme = 0;
          //  std::string test;
          //  input >> test;
            std::cout << i;
        printing = true;
            return 1;
        }
    }
    
    if (scheme == 0) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        activeHero->jump();
    }
    if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        std::shared_ptr<Ability> letters = std::make_shared<FallingLetters>(&activeHero->getWorld(), activeHero.get(), 6);

        if (activeHero->getComponent<CombatComponent>()->hasTarget()) {
            letters->setTarget(activeHero->getComponent<CombatComponent>()->getBigTarget());
        }
        activeHero->getComponent<CombatComponent>()->newAbility(letters);
    }
        if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
            std::vector<std::string> lines = {"WASD - Move", "don't touch E, R, T - old features that need new purpose", "Z - summon fish and break stuns", "X - swap world", "The boss ahead will dialogue you", "Then stun, press Z after", "Joseph Gu - Programmer", "Yirou Guo - Creative Consultant and Artist", "Jonathan Ran - Mathematical and Physics Consultant", "Matthew Ding - Deployment Help"};
            std::shared_ptr<Ability> speech = std::make_shared<Speech>(&activeHero->getWorld(), activeHero.get(), 6.0, lines);
            activeHero->getComponent<CombatComponent>()->newAbility(speech);
        }
        
        if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
            std::shared_ptr<Ability> fish = std::make_shared<Fish>(&activeHero->getWorld(), activeHero.get(), 18.0);
            activeHero->getComponent<CombatComponent>()->newAbility(fish);
        }
         
        if (key == GLFW_KEY_X && action == GLFW_PRESS) {
            game->swapWorld();
        } 
        
     /**   if (key == GLFW_KEY_E && action == GLFW_PRESS) {
            int newScheme = (-1)*(scheme-2);
            setActionScheme(newScheme);
            if (activeSketch.get() == NULL) {
                GLuint ftexture = 0;
            std::shared_ptr<Ability> sketch = std::make_shared<Sketch>(pHero0->getWorld(), pHero0.get(), 6, ftexture);
                pHero0->getComponent<CombatComponent>()->newAbility(sketch);
            }
            return 0;
        }
        
        if (key == GLFW_KEY_R && action == GLFW_PRESS) {
            GLuint ftexture = 0;
            glBindTexture(GL_TEXTURE_2D, ftexture);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 800, 800, GL_RGB, GL_UNSIGNED_BYTE, blank);
         //   glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 800, 800, GL_RGBA, GL_UNSIGNED_BYTE, blank);
            glBindTexture(GL_TEXTURE_2D, 0);
            if (activeSketch.get() != NULL) {
            abilities.erase(std::remove(abilities.begin(), abilities.end(), activeSketch));
            activeSketch.reset();
            }
        }
         **/
        if (key == GLFW_KEY_T && action == GLFW_PRESS) {
            if (activeSketch.get() != NULL)
            activeSketch->call2();
        }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
    //    inputHandler.setCharCallback(char_callback);
 //       inputHandler.setKeyCallback(onetap_callback0);
        scheme = 1;
    }
    }
    if (scheme == 2) {
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        int newScheme = (-1)*(scheme-2);
        setActionScheme(newScheme);
        firstMouse = true;
    }
    }
    return 0;
}

void InputHandler::setPlayerHero(const std::shared_ptr<Actor>& actor, int i) {
    if (i == 0)
        pHero0 = actor;
    
    if (i == 1)
        pHero1 = actor;
    
    if (activeHero.get() == NULL) {
        activeHero = pHero0;
    }
}

void InputHandler::setCamera(const std::shared_ptr<Camera>& cam) {
    activeCamera = cam;
}

void InputHandler::setActiveHero(int i) {
    if (i == 0) activeHero = pHero0;
    if (i == 1) activeHero = pHero1;
}

void InputHandler::setGame(Game* game_) {
    game = game_;
}
