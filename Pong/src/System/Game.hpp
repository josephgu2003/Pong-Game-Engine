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
#include "Ability.hpp"
#include "Sketch.hpp"
#include "Billow.hpp"
#include "Leaves.hpp"
#include "LoadingScreen.hpp"
#include "InkGlyphs.hpp"


class Game {
GLFWwindow* window; // Windowed
Camera camera;
World world;
Renderer* renderer;
InputHandler inputHandler;
    
    std::stringstream input;
    std::string i;
    bool printing = false;
   // boost::mutex mutex;
    
    int scheme = 0;
    double lastMX = 500;
    double lastMY = 400;
    double xOffset;
    double yOffset;
    bool firstMouse = true;
    bool mouseMoved = false;
    Sketch* activeSketch = NULL;
    
    Ball ball;
    PlayerHero pHero;
    Billow billow;
    Leaves leaves;
    InkGlyphs inkGlyphs;
    
    std::vector<Ability*> abilities;
    GLuint fbo, fvao, ftexture;
    unsigned char* paint;
    unsigned char* blank;
    int imageWidth = 0;
    int imageHeight = 0;
    int channels = 0;
    
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
};

#endif /* Game_hpp */