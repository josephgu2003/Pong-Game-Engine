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

class Game;

class InputHandler{ //detects input, lets game know
private:
    std::shared_ptr<Camera> activeCamera;
    std::shared_ptr<Actor> pHero0;
    std::shared_ptr<Actor> pHero1;
    std::shared_ptr<Actor> activeHero;
    
    std::string i;
    bool printing = false;
    std::shared_ptr<Dialogue> activeDialogue;
    std::shared_ptr<Sketch> activeSketch;
    
    GLFWwindow* window = NULL; 
    int scheme = 0;
    double lastMX = 500;
    double lastMY = 400;
    double xOffset;
    double yOffset;
    bool firstMouse = true;
    bool mouseMoved = false;
    
    Game* game = NULL;
public:
    InputHandler();
    ~InputHandler();
    void setWindow(GLFWwindow* window);
    
    void tick();
    void moveMouse(double mouseX_, double mouseY_);
    int processInput(int key, int action, int mods);
    
    void setGame(Game* game); //bad code hahaha 
    void setActionScheme(int id);
    void setPlayerHero(const std::shared_ptr<Actor>& actor, int i);
    void setActiveHero(int i);
    void setCamera(const std::shared_ptr<Camera>& cam);
};

#endif /* InputHandler_hpp */
