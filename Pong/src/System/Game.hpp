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
#include "PlayerHero.hpp"
#include "InputHandler.hpp"
#include <sstream>
#include <vector>
#include "Sketch.hpp"
#include "Billow.hpp"
#include "Mist.hpp"
#include "LoadingScreen.hpp"
#include "InkEffect.hpp"
#include "Audio.hpp"
#include "MapObject.hpp"
#include "Numberable.hpp"
#include "Fireworks.hpp"
#include "ScriptOne.hpp"
#include <chrono>

class Dialogue;

class Ability;

class Game {
GLFWwindow* window; // Windowed
Camera camera;
World world;
    World realWorld;
Renderer* renderer = NULL;
    Renderer* realRenderer = NULL;
    Renderer* activeRenderer = NULL;
    LoadingScreen* screen;
InputHandler inputHandler;
    Audio audio;
    
    std::stringstream input;
    std::string i;
    int nextBranch;
    bool printing = false;
   // boost::mutex mutex;
    
    int scheme = 0;
    double lastMX = 500;
    double lastMY = 400;
    double xOffset;
    double yOffset;
    bool firstMouse = true;
    bool mouseMoved = false;
    double lastTime = 0;
    
    std::shared_ptr<Sketch> activeSketch;
    Dialogue* activeDialogue = NULL;
    
    std::shared_ptr<Actor> ball;
    std::shared_ptr<Actor> pHero;
    std::shared_ptr<Actor> rHero;
    InkEffect inkGlyphs;
    Fireworks fireworks;
    MapObject map;
    MapObject realMap;
    std::shared_ptr<Actor> tree;
    Mist mist;
    
    ScriptOne* script;
    
    std::vector<std::shared_ptr<Ability>> abilities;
    Numberable* numberables[100];
    
    GLuint fbo, fvao, ftexture;
    unsigned char* paint = NULL;
    unsigned char* blank = NULL;
    int imageWidth = 0;
    int imageHeight = 0;
    int channels = 0;

    double fpsTimer;
    int intervalTimer = 0;
    bool firstTime = true;
    std::chrono::time_point<std::chrono::high_resolution_clock> t0;
    float draws;
    
    void initWindow();
    void initObjects();
    void linkObjects();
public:
    bool running = false;
    Game();
    ~Game();
    void tick();
    void moveMouse(double mouseX_, double mouseY_);
    int processInput(int key, int action, int mods);
    void processInput2(int key, int action);
    void print();
    void setActionScheme(int id);
    void newDialogue(Dialogue& dialogue_);
    
    Numberable* getNumberable(unsigned int ID_);
};

#endif /* Game_hpp */
