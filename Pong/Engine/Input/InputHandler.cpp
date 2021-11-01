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
#include "DialogueMenu.hpp"
#include "CharacterComponent.hpp"
#include "aiDialogueAction.hpp"

#define EMPTY_KEYCALLBACK [] (Game*, void* ref) {}

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
    setHandlerMode(KCALL_DEFAULT);
    game->getActivePlayerHero()->getComponent<NameComponent>()->speak(readText, 3.0);
    readText = ""; 
} 
 
void InputHandler::loadKeyCallbacks(KeyCallbackSet kcs) {
    if (!keyCallbacks->empty()) {
        keyCallbacks->clear();
    }
    switch (kcs) {
        case KCALL_DEFAULT: {
            setCallbackforKey(GLFW_KEY_ENTER, [](Game* game){
                auto x = game->getInputHandler();
                x.setHandlerMode(KCALL_READTEXT);
                x.loadKeyCallbacks(KCALL_READTEXT);
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
                std::vector<std::string> lines = {CREDITS};
                std::shared_ptr<Ability> speech = std::make_shared<Speech>(&ph->getWorld(), ph, 6.0, lines);
                ph->getComponent<CombatComponent>()->newAbility(speech);
            });
            
            setCallbackforKey(GLFW_KEY_M, [](Game* game){
                Actor* ph = game->getActivePlayerHero();
                std::vector<std::string> lines = {POEM};
                std::shared_ptr<Ability> speech = std::make_shared<Speech>(&ph->getWorld(), ph, 6.0, lines);
                ph->getComponent<CombatComponent>()->newAbility(speech);
            });
            
            setCallbackforKey(GLFW_KEY_Y, [](Game* game) {
                Actor* nearest;
                auto hero = game->getActivePlayerHero();
                if (!hero->getWorld().getNearestActorWith(hero, CHAR, nearest)) {
                    return;
                }
                std::shared_ptr<aiDialogueAction> dialogue = std::make_shared<aiDialogueAction>(10.0f, hero, nearest);
                std::shared_ptr<DialogueMenu> u = std::make_shared<DialogueMenu>(glm::vec2(0, 0), glm::vec2(0.5,0.5), "Resources/Textures/Project-10.png", dialogue);  
                game->getActivePlayerHero()->getComponent<CharacterComponent>()->newAction(dialogue);
                InputHandler& x = game->getInputHandler(); 
                game->getUI()->setActivePopup(u);
                x.addObserver(u);
                x.setHandlerMode(KCALL_MENU); 
                x.loadKeyCallbacks(KCALL_MENU);
            });
            
            auto shiftCall = [] (Game* g) {
                g->getInputHandler().swapCursorMode();
            };
            setCallbackforKey(GLFW_KEY_LEFT_SHIFT, shiftCall);
            break;
        }
            
        case KCALL_READTEXT: {
            setCallbackforKey(GLFW_KEY_ENTER, [](Game* game){
                auto x = game->getInputHandler();
                x.setHandlerMode(KCALL_DEFAULT);
                x.dumpTextToPlayer();
                x.loadKeyCallbacks(KCALL_DEFAULT);
            });
            break;
        }
        case KCALL_MENU: {
            setCallbackforKey(GLFW_KEY_ENTER, [](Game* game){
                auto x = game->getInputHandler();
                x.setHandlerMode(KCALL_DEFAULT);
                x.loadKeyCallbacks(KCALL_DEFAULT);
                game->getUI()->deleteActivePopup(); 
            });
            setCallbackforKey(GLFW_KEY_Y, [](Game* game) {
                auto x = game->getInputHandler();
                x.setHandlerMode(KCALL_DEFAULT);
                x.loadKeyCallbacks(KCALL_DEFAULT);
                game->getUI()->deleteActivePopup();
            });
            break;
        }
    }
}

InputHandler::InputHandler() {
    keyCallbacks = new std::map<int, keyCallback>();
    mode = KCALL_DEFAULT;
    mouseMovesCamera = true;
    readText = "";
    loadKeyCallbacks(KCALL_DEFAULT); 
  
}

InputHandler::~InputHandler() {
    
}

void InputHandler::setCallbackforKey(int i, keyCallback cbk) {
    if (keyCallbacks->find(i) == keyCallbacks->end()) {
        keyCallbacks->insert(std::pair<int, keyCallback>(i, cbk));
    } else { 
        keyCallbacks->find(i)->second = cbk;
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
    
    auto player = game->getActivePlayerHero();
    
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            player->posDir(0.03);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            player->posDir(-0.03);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            player->posRight(0.03);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            player->posRight(-0.03);
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
    auto call = keyCallbacks->find(ke.key);
    if (ke.action == GLFW_PRESS && call != keyCallbacks->end()) {
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
}


