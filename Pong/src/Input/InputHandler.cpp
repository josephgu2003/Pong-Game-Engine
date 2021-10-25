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
#include "uiText.hpp"
#include "uiMenu.hpp"

#define EMPTY_KEYCALLBACK [] (Game* ) {}

#define POEM "I was asked - \"Do you have dreams?\"", "No...", "...Yes? Lost. Searching. Searching.", "Searching with colorful moonlight always overhead,","Yet my eyes were always down, scouring that dark canvas.","Too late, gaze up at the painted moon.", "A flash of inspiration, and the coldness of regret.","Is it too late? The moon is going away soon.","A brush dipped in lost dreams refound,", "But a hand still with regretfulness.","If only I had a pond, so that by its reflection,","I would have seen the moon's beauty sooner.","A brush, a canvas, a horizon","An artist dreaming of the moon."

#define CREDITS "WASD - Move", "don't touch E, R, T - old features that need new purpose", "Z - summon fish and break stuns", "X - swap world", "The boss ahead will dialogue you", "Then stun, press Z after", "Joseph Gu - Programmer", "Yirou Guo - Creative Consultant and Artist", "Jonathan Ran - Mathematical and Physics Consultant", "Matthew Ding - Deployment Help"

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
    readText = ""; 
}

InputHandler::InputHandler() {
    readText = "";
    
    auto enterA = [](Game* game){
        game->getInputHandler().setReadTextMode(true);
    };
    auto enterB = [](Game* game){
        game->getInputHandler().dumpTextToPlayer();
    };
    
    auto enterCall = InputCallback(enterA, enterB);
    enterCall.enableToggle();
    
    setCallbackforKey(GLFW_KEY_ENTER, enterCall);
    
    setCallbackforKey(GLFW_KEY_X, InputCallback([](Game* game){
        game->swapWorld();
    }, EMPTY_KEYCALLBACK));
    
    setCallbackforKey(GLFW_KEY_SPACE, InputCallback([](Game* game){
        game->getActivePlayerHero()->jump();
    }, EMPTY_KEYCALLBACK));

    setCallbackforKey(GLFW_KEY_Z, InputCallback([](Game* game){
        Actor* ph = game->getActivePlayerHero();
        std::shared_ptr<Ability> fish = std::make_shared<Fish>(&ph->getWorld(), ph, 18.0);
        ph->getComponent<CombatComponent>()->newAbility(fish);
    }, EMPTY_KEYCALLBACK));
    
    setCallbackforKey(GLFW_KEY_G, InputCallback([](Game* game){
        Actor* ph = game->getActivePlayerHero();
        std::shared_ptr<Ability> letters = std::make_shared<FallingLetters>(&ph->getWorld(), ph, 6.0);
        auto comb = ph->getComponent<CombatComponent>();
        if (comb->hasTarget()) {
            letters->setTarget(comb->getBigTarget());
        }
        comb->newAbility(letters);
    }, EMPTY_KEYCALLBACK));
    
    setCallbackforKey(GLFW_KEY_Q, InputCallback([](Game* game){
        Actor* ph = game->getActivePlayerHero();
        std::vector<std::string> lines = {CREDITS};
        std::shared_ptr<Ability> speech = std::make_shared<Speech>(&ph->getWorld(), ph, 6.0, lines);
        ph->getComponent<CombatComponent>()->newAbility(speech);
    }, EMPTY_KEYCALLBACK));
    
    setCallbackforKey(GLFW_KEY_M, InputCallback([](Game* game){
        Actor* ph = game->getActivePlayerHero();
        std::vector<std::string> lines = {POEM};
        std::shared_ptr<Ability> speech = std::make_shared<Speech>(&ph->getWorld(), ph, 6.0, lines);
        ph->getComponent<CombatComponent>()->newAbility(speech);
    }, EMPTY_KEYCALLBACK));
    
    auto yCall = InputCallback(
[](Game* game){
        std::shared_ptr<uiMenu> u = std::make_shared<uiMenu>(glm::vec2(0, 0), glm::vec2(0.5,0.5), TEX_UIMENU_1);
        game->getUI()->insertNode(u);
        std::shared_ptr<uiText> t1 = std::make_shared<uiText>("Option 1", 0.25, 0.45);
        std::shared_ptr<uiText> t2 = std::make_shared<uiText>("Option 2", 0.25, 0.35);
        std::shared_ptr<uiText> t3 = std::make_shared<uiText>("Option 3", 0.25, 0.25);
        game->getUI()->insertNode(t1);
        game->getUI()->insertNode(t2);
        game->getUI()->insertNode(t3);},
[](Game* game){
        // delete menu
        
    });
    yCall.enableToggle();
                               
    setCallbackforKey(GLFW_KEY_Y, yCall);
    
    auto shiftCall = InputCallback(
[] (Game* g) {
        g->getInputHandler().setCursorMode(GLFW_CURSOR_NORMAL);
    }, [] (Game* g) {
        g->getInputHandler().setCursorMode(GLFW_CURSOR_DISABLED);
    });
    shiftCall.enableToggle();  
    setCallbackforKey(GLFW_KEY_LEFT_SHIFT, shiftCall);

}

InputHandler::~InputHandler() {
    
}

void InputHandler::setCallbackforKey(int i, const InputCallback& cbk) {
    if (keyCallbacks.find(i) == keyCallbacks.end()) {
        keyCallbacks.insert(std::pair<int, InputCallback>(i, cbk));
    } else {
        keyCallbacks.find(i)->second.undoEffects(game);
        keyCallbacks.find(i)->second = cbk;
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
            keyEventQ.pop();
            
            if (ke.action == GLFW_PRESS)  {
                if (ke.key == GLFW_KEY_ENTER) {
                    processInput(ke);
                }
                const char* key_name = glfwGetKeyName(ke.key, 0);
                if (!key_name) continue;
                readText.append(key_name);
            }
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

void InputHandler::setCursorMode(GLenum g) {
    glfwSetInputMode(window, GLFW_CURSOR, g);
}

int InputHandler::processInput(const KeyEvent& ke) {
    auto call = keyCallbacks.find(ke.key);
    if (ke.action == GLFW_PRESS && call != keyCallbacks.end()) {
        call->second.execute(game);
        return 0;
    }
    
    
    
   // int key = ke.key;
   // int action = ke.action;
   // int mod = ke.mod;
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
