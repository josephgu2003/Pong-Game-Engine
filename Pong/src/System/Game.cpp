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
#include "JsonManager.hpp"
#include "AssetManager.hpp"
#include "HealthMeter.hpp"
#include "LifeComponent.hpp"
#include "Subject.hpp"
 
Game::Game() {
    ui = std::make_shared<uiLayout>();
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
    
    screen = new LoadingScreen();
    
    inputHandler.setWindow(window);
    inputHandler.setGame(this);

    stbi_set_flip_vertically_on_load(0);
    
    initObjects();
  
    linkObjects();
    
    JsonManager::loadGame(this);
    
    camera->setActor(pHero0.get());

  
    screen->print("Preparing the brushes...");
    glfwPollEvents();
    glfwSwapBuffers(window);

    
    screen->print("Opening the books...");
    glfwPollEvents();
    glfwSwapBuffers(window);
    
    renderer0->loadSkyBoxData();
    
    screen->print("Putting on a fresh canvas...");
    glfwPollEvents();
    glfwSwapBuffers(window);

    renderer1->loadSkyBoxData();
    
    screen->print("Flipping the pages...");
    glfwPollEvents();
    glfwSwapBuffers(window);
    
    stbi_set_flip_vertically_on_load(1);

    printf("%s\n", glGetString(GL_VERSION));
    
    
    std::shared_ptr<HealthMeter> hm =  std::make_shared<HealthMeter>();
    pHero0->getComponent<LifeComponent>()->addObserver(hm);
    ui->insertNode(hm);

    activeRenderer = renderer0;
    activeWorld = &world0; 
    inputHandler.setActiveHero(pHero0);
    activeHero = pHero0; 

    audio.playMusic();
        
    script = new ScriptOne();  
    script->init(this); 
 
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
    camera = std::make_shared<Camera>();
    renderer1 = new Renderer; 
     renderer0 = new Renderer;
     
    mist = new ParticleSystem();
    
    inkGlyphs = new ParticleSystem();
    
    fireworks = new ParticleSystem();
    
    mist->init(PE_MIST, glm::vec3(0,1.0,0));
       
    fireworks->init(PE_FIREWORKS, glm::vec3(0,27,0));
    
    inkGlyphs->init(PE_BODYSPARKS, glm::vec3(0,1.0,0)); 
    // realMap.init(glm::vec3(0,-1.0,0));
     //map.init(glm::vec3(0,-0.14,0));

}
                   
void Game::linkObjects() {      
    renderer0->setWorld(&world0);
    renderer1->setWorld(&world1);
                 
    world0.setRenderer(renderer0);
    world1.setRenderer(renderer1);
    
    DirectionalLight  dl2(glm::vec3(0.2,0.2,0.2),glm::vec3(0.8,0.8,0.8),glm::vec3(1.0,1.0,1.0),glm::vec3(-1,-1,0));
    world1.setWeather(dl2, 0);  
         
    DirectionalLight   dl(glm::vec3(0.03,0.03,0.03),glm::vec3(0.08,0.08,0.08),glm::vec3(0.5,0.5,0.5),glm::vec3(-1,-1,0));
    world0.setWeather(dl, 0);

    renderer1->setCamera(camera.get());
    renderer0->setCamera(camera.get());
 
    world0.insertCamera(camera.get());
        
    world0.insertParticleEffect(fireworks);
    world0.insertParticleEffect(mist);

    world0.insertParticleEffect(inkGlyphs);
   // world0.setMap(map);
   // world1.setMap(realMap);
 
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

    script->tick();
    
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
        inputHandler.setActiveHero(pHero1);
        activeHero = pHero1;
        activeWorld = &world1;
        activeRenderer = renderer1;
        activeRenderer->updateLights();
        camera->setActor(pHero1.get());
        return;
    }
    if (activeWorld == &world1) {
        inputHandler.setActiveHero(pHero0);
        activeHero = pHero0;
        activeWorld = &world0;
        activeRenderer = renderer0;
        activeRenderer->updateLights();
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
 
