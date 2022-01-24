//
//  Game.cpp
//  Pong
//
//  Created by Joseph Gu on 6/3/21.
//
 
#include "Game.hpp"
#include <iostream>
#include <cctype>
#include "stb_image.h" 
#include "json.hpp"
#include "AssetManager.hpp"
#include "JsonManager.hpp"
#include "Shader.hpp"

Game::Game() { 
    glfwInit(); 
    running = true;
    
    initWindow();
    
    glewExperimental = GL_TRUE;
    glewInit();
    
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LEQUAL);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetTime(0);
    glfwSetWindowUserPointer(window, &inputHandler);

    stbi_set_flip_vertically_on_load(1);
    
    initObjects();
  
    linkObjects();

    printf("%s\n", glGetString(GL_VERSION));
}

void Game::registerGameLevelCreate(std::string levelname, GameLevelCreate glc) {
    auto entry = levelBuilder.find(levelname);
    if (entry != levelBuilder.end()) {
        entry->second = glc;
    } else {
        levelBuilder.insert(std::pair<std::string, GameLevelCreate>(levelname, glc));
    }
}

void Game::initWindow() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);   
        
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window); 
}
  
void Game::initObjects() { 
    ui = std::make_shared<uiLayout>(); 
    camera = std::make_shared<Camera>(); 
    renderer = new Renderer();
    saveSystem = std::unique_ptr<SaveSystem>(); 
    activeLevel = std::make_unique<GameLevel>(renderer, 1, "placeholder");
} 
                    
void Game::linkObjects() {  
    inputHandler.setWindow(window);
    inputHandler.setGame(this);
    renderer->setCamera(camera.get());
}

Game::~Game() {
     
}

void Game::tick() {
    if (!fpsControl.regulateFPS()) {
        return;
    } 
    inputHandler.tick();

    if(glfwWindowShouldClose(window)) {
        running = false;
    } 

    renderer->renderInitial();
       
    std::string nextLvl = activeLevel->tick();
    if (nextLvl != "") {
        loadLevel(nextLvl);
    }
    ui->renderAll(renderer); 
    renderer->renderFinal();

    glfwPollEvents(); 
    glfwSwapBuffers(window);
}
 

void Game::end() {
    saveLevel(activeLevel.get());
    glfwTerminate(); 
}

InputHandler& Game::getInputHandler() {
    return inputHandler;
}


std::shared_ptr<uiLayout>& Game::getUI() {
    return ui;
}
 
void Game::init() { 
    load();
}
    
void Game::load() {
    
}
  
GameLevel* Game::getActiveLevel() {
    return activeLevel.get();
}

void Game::setLevelBuilder(LevelBuilder lvlBuilder) {
    levelBuilder = lvlBuilder; 
}

void Game::loadLevel(std::string lvl) {
    auto lvlCreate = levelBuilder.find(lvl);
    if (lvlCreate != levelBuilder.end()) {
        activeLevel.reset(); // important to let deconst
        activeLevel.reset((lvlCreate->second)(this));
        if (auto x = activeLevel->getActiveWorld()->getPlayerHero()) {
            camera->setActor(activeLevel->getActiveWorld()->getPlayerHero());
        }
        activeLevel->getActiveWorld()->insert<Camera>(camera);
    } 
}

Renderer* Game::getRenderer() {
    return renderer; 
}
 

Actor* Game::getPlayerHero() {
    if (auto level = activeLevel.get()) {
        if (auto world = level->getActiveWorld()) {
            if (auto player = world->getPlayerHero())
                return player;
        }
    }
    return nullptr;
}

void Game::setSaveSystem(SaveSystem* s) {

    saveSystem.reset(s);
}

void Game::loadLevelSaveFile(GameLevel* g) {
    saveSystem->loadGameLevel(g);
}

void Game::saveLevel(GameLevel* g) {
    saveSystem->saveGameLevel(g);
} 
 
