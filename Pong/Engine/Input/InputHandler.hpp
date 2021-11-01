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
#include "Subject.hpp"
 
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


enum KeyCallbackSet{
    KCALL_DEFAULT,
    KCALL_READTEXT,
    KCALL_MENU
};


class InputHandler : public Subject { //detects input, executes associated action
private:
    std::queue<KeyEvent> keyEventQ;
    std::map<int, keyCallback>* keyCallbacks = NULL;
    
    std::shared_ptr<Camera> activeCamera;
    
    GLFWwindow* window = NULL;
    
    double lastMX = 500;
    double lastMY = 400;
    bool firstMouse = true;
    
    bool mouseMovesCamera;
    std::string readText;
    GLenum currentKey;
    KeyCallbackSet mode;
    
    Game* game = NULL;
    void setCallbackforKey(int i, keyCallback cbk);
    void clearCallbackforKey(int i);
    void clearCallbacks();
    int processInput(const KeyEvent& ke);
    void loadKeyCallbacks(KeyCallbackSet kcs);
public:
    InputHandler();
    ~InputHandler();
    void setWindow(GLFWwindow* window);
    void dumpTextToPlayer(); //dont need
     
    void tick();
    void moveMouse(double mouseX_, double mouseY_);
    void addKeyEventToQ(int key, int action, int mods);
    
    void setGame(Game* game); //bad code hahaha
    void setCamera(const std::shared_ptr<Camera>& cam);
    void swapCursorMode();
    GLenum getCurrentKeyPress() const;
    void setHandlerMode(KeyCallbackSet set);
};

#endif /* InputHandler_hpp */
 
// have multiple layouts, some transition to each other, now u dont need to set
// random find ur way backs, worry about a button being overriden by another and
// unable to execut findurwayback, no worrying about a menu being

// original approach : swappable buttons, has problems with toggles and buttons being swapped mid actions, leaving dangling actions
// also problem with button references -> lets leave references to game object
//
