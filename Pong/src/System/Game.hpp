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
#include "Sketch.hpp"
#include "Mist.hpp"
#include "LoadingScreen.hpp"
#include "InkEffect.hpp"
#include "MapObject.hpp"
#include "Fireworks.hpp"
#include "ScriptOne.hpp"
#include "Watch.hpp"
#include "uiLayout.hpp"
#include "Audio.hpp"


class Game {
private:
    Watch watch;
    Watch timerForTick;
    
    GLFWwindow* window; // Windowed
    LoadingScreen* screen;
    
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
    
    double lastTime = 0;
    double fpsTimer;
    int intervalTimer = 0;
    bool firstTime = true;
    float draws;
    
    InkEffect inkGlyphs;
    Fireworks fireworks;
    MapObject map;
    MapObject realMap;
    Mist mist;
    ScriptOne* script;
    
    void initWindow();
    void initObjects();
    void linkObjects();
public:
    bool running = false;
    Game();
    ~Game();
    void tick();

   // void setNumberable(Numberable* numberable, int i);
   // Numberable* getNumberable(unsigned int ID_);
    World& getWorld(int i);
    void setPlayerHero(const std::shared_ptr<Actor>& actor, int i);
    Actor* getActivePlayerHero();
    void end();
    InputHandler& getInputHandler();
    void swapWorld();
};

#endif /* Game_hpp */
