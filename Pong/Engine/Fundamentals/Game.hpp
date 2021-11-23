//
//  Game.hpp
//  Pong
//
//  Created by Joseph Gu on 6/3/21.
//

#ifndef Game_hpp
#define Game_hpp

#include <GL/glew.h>
#define GLFW_DLL
#include "Renderer.hpp"
#include "Camera.hpp"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "Actor.hpp"
#include "World.hpp"
#include "InputHandler.hpp"
#include <vector>
#include "MapObject.hpp"
#include "ScriptOne.hpp"
#include "Watch.hpp"
#include "uiLayout.hpp"
#include "Audio.hpp"

class Game {
private:
    double fpsTimer;
    int intervalTimer = 0;
    bool firstTime = true;
    float draws;
    Watch timerForTick;
    virtual void load();
protected:
    Watch watch;
    
    GLFWwindow* window; // Windowed
    
    std::shared_ptr<Camera> camera;
    std::shared_ptr<uiLayout> ui;
    
    World world0;
    World world1;
    
    World* activeWorld = NULL;
    
    Renderer* renderer0 = NULL;
    Renderer* renderer1 = NULL;
    Renderer* activeRenderer = NULL;
     
    InputHandler inputHandler;
    
    Audio audio;
       
    std::shared_ptr<Actor> pHero0; 
    std::shared_ptr<Actor> pHero1;
    std::shared_ptr<Actor> activeHero;
    
    void initWindow();
    void initObjects();
    void linkObjects();
public:
    bool running = false;
    Game();
    ~Game();
    void tick(); 
    void init();
    World& getWorld(int i);
    void setPlayerHero(const std::shared_ptr<Actor>& actor, int i);
    Actor* getActivePlayerHero();
    void end();
    InputHandler& getInputHandler();
    void swapWorld();
    std::shared_ptr<uiLayout>& getUI();
    
};

#endif /* Game_hpp */
