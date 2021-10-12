
//  main.cpp
//  Pong
//
//  Created by Joseph Gu on 4/24/21.
//

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <iostream>
#include "Game.hpp"



int main() {
    
    glfwInit();
    
    Game game;

    
    while(game.running)

    {
        game.tick();
    }
    
    game.end();
    glfwTerminate();
    
    return 0;
    
}

/**
 don't copy large amounts of data
 use same vertices even when not instancing
 
 notes on performance:
 5/24: 7 % w/o, 8 % with particles of minecraft (200), no instancing
 6/3 : 18%, 713 mb 2000 particles
 6/12 :20%, 1.6 gb
 **/
