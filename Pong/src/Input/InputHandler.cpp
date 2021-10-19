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
#include "NameComponent.hpp"
#include "CombatComponent.hpp"

void onetap_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    InputHandler* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
    handler->addKeyEventToQ(key, action, mods);
}

void mouse_callback(GLFWwindow* window, double mouseX_, double mouseY_) {
    InputHandler* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
    handler->moveMouse(mouseX_, mouseY_);
}

void InputHandler::dumpTextToPlayer() {
    setReadTextMode(false);
    game->getActivePlayerHero()->getComponent<NameComponent>()->speak(readText, 3.0);
    setCallbackforKey(GLFW_KEY_ENTER,  [](Game* game){
        game->getInputHandler().setReadTextMode(true);
        game->getInputHandler().setCallbackforKey(GLFW_KEY_ENTER, [](Game* game){
            game->getInputHandler().dumpTextToPlayer();
        });
    });
    readText = ""; 
}

InputHandler::InputHandler() {
    readText = "";
    
    setCallbackforKey(GLFW_KEY_ENTER,  [](Game* game){
        game->getInputHandler().setReadTextMode(true);
        game->getInputHandler().setCallbackforKey(GLFW_KEY_ENTER, [](Game* game){
            game->getInputHandler().dumpTextToPlayer();
        });
    });
    
    setCallbackforKey(GLFW_KEY_X, [](Game* game){
        game->swapWorld();
    });
    
    setCallbackforKey(GLFW_KEY_SPACE, [](Game* game){
        game->getActivePlayerHero()->jump();
    });

    setCallbackforKey(GLFW_KEY_Z, [](Game* game){
        Actor* ph = game->getActivePlayerHero();
        std::shared_ptr<Ability> fish = std::make_shared<Fish>(&ph->getWorld(), ph, 18.0);
        ph->getComponent<CombatComponent>()->newAbility(fish);
    });
    
    setCallbackforKey(GLFW_KEY_G, [](Game* game){
        Actor* ph = game->getActivePlayerHero();
        std::shared_ptr<Ability> letters = std::make_shared<FallingLetters>(&ph->getWorld(), ph, 6.0);
        auto comb = ph->getComponent<CombatComponent>();
        if (comb->hasTarget()) {
            letters->setTarget(comb->getBigTarget());
        }
        comb->newAbility(letters);
    });
    
    setCallbackforKey(GLFW_KEY_Q, [](Game* game){
        Actor* ph = game->getActivePlayerHero();
        std::vector<std::string> lines = {"WASD - Move", "don't touch E, R, T - old features that need new purpose", "Z - summon fish and break stuns", "X - swap world", "The boss ahead will dialogue you", "Then stun, press Z after", "Joseph Gu - Programmer", "Yirou Guo - Creative Consultant and Artist", "Jonathan Ran - Mathematical and Physics Consultant", "Matthew Ding - Deployment Help"};
        std::shared_ptr<Ability> speech = std::make_shared<Speech>(&ph->getWorld(), ph, 6.0, lines);
        ph->getComponent<CombatComponent>()->newAbility(speech);
    });
    

}

InputHandler::~InputHandler() {
    
}

void InputHandler::setCallbackforKey(int i, keyCallback kc) {
    if (keyCallbacks.find(i) == keyCallbacks.end()) {
        keyCallbacks.insert(std::pair<int, keyCallback>(i, kc));
    } else {
        keyCallbacks.find(i)->second = kc; 
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

// input handler read mode turned on
// read, until enter is hit again
// text dumped to actor speech?

// something calls read mode, and accepts the string?, including inputhanlder itself

// turnOnReadMode(string ref?); observer pattern can't use?

void InputHandler::tick() {
    if (readKeysToTextMode) { // read text isntead of controls
        while (!keyEventQ.empty()) {
            KeyEvent& ke = keyEventQ.front();
            if (ke.key == GLFW_KEY_ENTER) {
                processInput(ke);
            }
            const char* key_name = glfwGetKeyName(ke.key, 0);
            keyEventQ.pop();
            if (!key_name) continue;
            readText.append(key_name);
        }
        return;
    }
    while (!keyEventQ.empty()) {
        KeyEvent& ke = keyEventQ.front();
        processInput(ke);
        keyEventQ.pop();
    }
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
     
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}



void InputHandler::moveMouse(double mouseX_, double mouseY_) {
    if (firstMouse) {
        lastMX = mouseX_;
        lastMY = mouseY_;
        firstMouse = false;
    }
    xOffset = mouseX_ - lastMX;
    yOffset = lastMY - mouseY_;
    lastMX = mouseX_;
    lastMY = mouseY_;
    activeCamera->rotate(glm::vec3(yOffset*0.03, xOffset*0.03, 0));
    xOffset = 0;
    yOffset = 0;
    
}

void InputHandler::setReadTextMode(bool b) {
    readKeysToTextMode = b;
}

int InputHandler::processInput(const KeyEvent& ke) {
    auto call = keyCallbacks.find(ke.key);
    if (ke.action == GLFW_PRESS && call != keyCallbacks.end()) {
        (*(call->second))(game); 
        return 0;
    }
    
    int key = ke.key;
    int action = ke.action;
    int mod = ke.mod; 
//    unique_lock<mutex> lock() 
 /**   if (scheme == 3) {
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
        if (mod == GLFW_MOD_SHIFT){
        char c = toupper(*key_name);
            if (key == GLFW_KEY_1) c = '!';
            if (key == GLFW_KEY_SLASH) c = '?';
            if (key == GLFW_KEY_APOSTROPHE) c = '\"';
        i.append(std::string(1, c));
         } else if (mod != GLFW_MOD_SHIFT) {
            i.append(key_name);
         } 
    }
        
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
            scheme = 0;
          //  std::string test;
          //  input >> test;
        printing = true;
            return 1;
        }
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
    }**/
    return 0;
}


void InputHandler::setCamera(const std::shared_ptr<Camera>& cam) {
    activeCamera = cam;
}

void InputHandler::setActiveHero(const std::shared_ptr<Actor>& actor) {
    activeHero = actor;
}

void InputHandler::setGame(Game* game_) {
    game = game_;
}
