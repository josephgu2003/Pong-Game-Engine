//
//  Game.cpp
//  Pong
//
//  Created by Joseph Gu on 6/3/21.
//
#define WINDOW_WIDTH 2000
#define WINDOW_HEIGHT 1300

#include "Game.hpp"
#include <iostream>
#include <cctype>
#include "stb_image.h"
#include <thread>
#include "json.hpp"
#include "AssetManager.hpp"
#include "JsonManager.hpp"

Game::Game() { 
    watch.resetTime();
    timerForTick.resetTime();
    running = true;
    
    initWindow();
    
    glewExperimental = GL_TRUE;
    glewInit(); 
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LEQUAL);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetTime(0);
    glfwSetWindowUserPointer(window, &inputHandler);

    stbi_set_flip_vertically_on_load(0);
    
    initObjects();
  
    linkObjects();
    
    renderer1->setCamera(camera.get());
    renderer0->setCamera(camera.get());

    printf("%s\n", glGetString(GL_VERSION));

    activeRenderer = renderer0;
    activeWorld = &world0;
    
    renderer0->loadSkyBoxData();
    renderer1->loadSkyBoxData();

    audio.playMusic();
}

void Game::initWindow() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    window = glfwCreateWindow(0.5*WINDOW_WIDTH, 0.5*WINDOW_HEIGHT, "OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
}
  
void Game::initObjects() { 
    ui = std::make_shared<uiLayout>(); 
    camera = std::make_shared<Camera>();
    renderer1 = new Renderer;
    renderer0 = new Renderer;

}
                   
void Game::linkObjects() {
    inputHandler.setWindow(window);
    inputHandler.setGame(this);
    
    renderer0->setWorld(&world0);
    renderer1->setWorld(&world1);
                 
    world0.setRenderer(renderer0);
    world1.setRenderer(renderer1);
 
    world0.insertCamera(camera.get());
    
    inputHandler.setCamera(camera);
}

 
Game::~Game() {
     
}

void Game::tick() {
    float ratio = (float) watch.getTime()/0.030f; 
    if (floor(ratio) == intervalTimer) {
        intervalTimer = floor(ratio);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        return;
    }
    else {
        intervalTimer = floor(ratio);
    }
     
    float elapsedT = timerForTick.getTime();
    glfwSetTime(elapsedT);
    timerForTick.resetTime();
    fpsTimer += elapsedT;
    
    draws = draws + 1.0;     
    if (fpsTimer > 2.0) {
        float fps = draws/2.0;
        printf("Fps is %f\n", fps);
        fpsTimer = 0.0;
        draws = 0.0;  
    }

    inputHandler.tick();

    if(glfwWindowShouldClose(window)) {
        running = false;
    }
    
    if (activeRenderer == renderer0) { // yes MAKES NO SENSE GO FIX IT
        world1.tick();
        activeRenderer->render();
        world0.tick();
          
        ui->renderAll(activeRenderer);
        activeRenderer->render2();
    } else {
        world0.tick(); 
        
        activeRenderer->render();

        world1.tick();
    
        ui->renderAll(activeRenderer);
        activeRenderer->render2();
    }

    glfwPollEvents();
    glfwSwapBuffers(window);
}


World& Game::getWorld(int i) {
    if (i == 0) return world0;
    if (i == 1) return world1; 
    return world0; 
}

void Game::setPlayerHero(const std::shared_ptr<Actor>& actor, int i) {
    if (i == 0)
        pHero0 = actor; 
    
    if (i == 1)
        pHero1 = actor;
    
} 

void Game::end() {
    JsonManager::saveGame(this);
}

InputHandler& Game::getInputHandler() {
    return inputHandler;
}

void Game::swapWorld() {
    if (activeWorld == &world0) {
        activeHero = pHero1;
        activeWorld = &world1;
        activeRenderer = renderer1;
        activeRenderer->updateLights();
        if(pHero1.get())
        camera->setActor(pHero1.get());
        return;
    }
    if (activeWorld == &world1) {
        activeHero = pHero0;
        activeWorld = &world0;
        activeRenderer = renderer0;
        activeRenderer->updateLights();
        if (pHero0.get())
        camera->setActor(pHero0.get());
        return;
    }
}
  
 
Actor* Game::getActivePlayerHero() {
    return activeHero.get();
}

std::shared_ptr<uiLayout>& Game::getUI() {
    return ui;
}
 
void Game::init() {
    load();
    activeHero = pHero0;
}
  
void Game::load() {
    
}
