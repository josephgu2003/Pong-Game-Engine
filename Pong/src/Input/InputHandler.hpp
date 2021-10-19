//
//  InputHandler.hpp
//  Pong
//
//  Created by Joseph Gu on 6/3/21.
//

#ifndef InputHandler_hpp
#define InputHandler_hpp

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "Actor.hpp"
#include "Camera.hpp"
#include "Dialogue.hpp"
#include "Sketch.hpp"
#include <memory>
#include <map>
#include <queue>
 
class Game;

typedef void (*keyCallback) (Game*);

struct KeyEvent {
    int key;
    int action;
    int mod;
    KeyEvent(int key_, int action_, int mods_) {
        key = key_;
        action = action_; 
        mod = mods_;
    }
};
class InputHandler { //detects input, executes associated action
private:
    std::queue<KeyEvent> keyEventQ;
    std::map<int, keyCallback> keyCallbacks;
    std::shared_ptr<Camera> activeCamera;     
    std::shared_ptr<Actor> activeHero; 
    
    std::shared_ptr<Dialogue> activeDialogue; 
    std::shared_ptr<Sketch> activeSketch;
    
    GLFWwindow* window = NULL;
    
    double lastMX = 500;
    double lastMY = 400;
    double xOffset;
    double yOffset;
    bool firstMouse = true; 
    bool mouseMoved = false; 
    
    bool readKeysToTextMode = false;
    std::string readText;
    
    Game* game = NULL;
    void setCallbackforKey(int i, keyCallback); 
public:
    InputHandler();
    ~InputHandler();
    void setWindow(GLFWwindow* window);
    void setReadTextMode(bool b);
    void dumpTextToPlayer();
     
    void tick();
    void moveMouse(double mouseX_, double mouseY_);
    int processInput(const KeyEvent& ke);
    
    void setGame(Game* game); //bad code hahaha 
    void setActiveHero(const std::shared_ptr<Actor>& act);
    void setCamera(const std::shared_ptr<Camera>& cam);
    void addKeyEventToQ(int key, int action, int mods);
};

#endif /* InputHandler_hpp */
