
//  main.cpp
//  Pong
//
//  Created by Joseph Gu on 4/24/21.
//

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <iostream>
#include "TicTacToe.hpp"
#include "Game.hpp"

int main() {
    glfwInit();
    
    Game game;

    while(game.running)
    {
        game.tick();
    }
    glfwTerminate();
    return 0;
}


/**work on:
// fix that bloody ability interface
 // ask dad about bool flags
 // destructor for ability
 
 notes on performance:
 5/24: 7 % w/o, 8 % with particles of minecraft (200), no instancing
 6/3 : 18%, 713 mb 2000 particles
 **/

// design: game system (hold data of loaded models actors scene etc), graphics system (take the scene and draw it with all the right configurations), audio, assetmanager system, input system (i think this one should handle player actions instead of functions in playerhero class?)
