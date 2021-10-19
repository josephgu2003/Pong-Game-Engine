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
    
    
    inkGlyphs.setActor(pHero0.get());
    camera->setActor(pHero0.get());
    
    VertexData* data = new VertexData;
    AnyVertex* a = new TBNVertex(glm::vec3(-50,0,-50),glm::vec3(0,1,0),glm::vec2(0,0), glm::vec3(0,0,0), glm::vec3(0,0,0));
    AnyVertex* b = new TBNVertex(glm::vec3(50,0,-50),glm::vec3(0,1,0),glm::vec2(1,0), glm::vec3(0,0,0), glm::vec3(0,0,0));
    AnyVertex* c = new TBNVertex(glm::vec3(50,0,50),glm::vec3(0,1,0),glm::vec2(1,1), glm::vec3(0,0,0), glm::vec3(0,0,0));
    AnyVertex* d = new TBNVertex(glm::vec3(-50,0,50),glm::vec3(0,1,0),glm::vec2(0,1), glm::vec3(0,0,0), glm::vec3(0,0,0));
    std::vector<AnyVertex*> mapVertices = {a, b, c, d
    };
    
    std::vector<GLuint> mapIndices = {
        0, 1, 2,
        2, 3, 0
    };
     
    TextureMaps mapTextures;
    Texture t;
    AssetManager::loadNullTexture(1600, 1600, &t.id, GL_RGBA);
    mapTextures.diffuse = (t);
    data->setVertexData(mapVertices, mapIndices, mapTextures, VERTEX_TBNVERTEX);
    Shader* shader = new Shader("Shaders/WaterVertexShader.vs", "Shaders/WaterFragmentShader.fs");
    glm::mat4 modelMat = glm::mat4(1.0);
    modelMat = glm::translate(modelMat, glm::vec3(map.getPos()));
    
    shader->use();
    shader->setMat4("modelMat", modelMat);
    glm::mat3 mat = glm::mat3(glm::transpose(glm::inverse(modelMat)));
    shader->setMat3("transposeInverseModelMat", mat);
    GraphicsComponent* graphics = new GraphicsComponent(data, shader);
    map.setGraphics(graphics);
     
    Renderer::bindShaderUniblock(shader, ViewProj);
    Renderer::bindShaderUniblock(shader,   Lights);
    Renderer::bindShaderUniblock(shader, StopWatch);
 //memeleak
    
    Model*  model = new Model();
    AssetManager::loadModel("Resources/Map/snow3.obj", model);
    TextureMaps maps2;
    
    AssetManager::loadTexture(TEX_VORONOI, &maps2.voronoi, false);

    model->setMeshTexture(0, maps2);
    VertexData* data2 = &model->getMeshes()->at(0);
    Shader* shader2 = new Shader("Shaders/SnowVertexShader.vs", "Shaders/SnowFragmentShader.fs");
    modelMat = glm::mat4(1.0);
    modelMat = glm::translate(modelMat, glm::vec3(realMap.getPos()));
    shader2->use();
    shader2->setMat4("modelMat", modelMat);
    shader2->setFloat("size", 15); 
    mat = glm::mat3(glm::transpose(glm::inverse(modelMat)));
    shader2->setMat3("transposeInverseModelMat", mat);
    GraphicsComponent* graphics2 = new GraphicsComponent(data2, shader2);
    realMap.setGraphics(graphics2);
  
    Renderer::bindShaderUniblock(shader2,    ViewProj);
    Renderer::bindShaderUniblock(shader2,    Lights); 
    Renderer::bindShaderUniblock(shader2,     StopWatch); 

    
    screen->print("Preparing the brushes...");
    glfwPollEvents();
    glfwSwapBuffers(window);
    
    renderer0->loadActorData();
    
    screen->print("Opening the books...");
    glfwPollEvents();
    glfwSwapBuffers(window);
    
    renderer0->loadMapData();
    renderer0->loadSkyBoxData();
    
    screen->print("Putting on a fresh canvas...");
    glfwPollEvents();
    glfwSwapBuffers(window);
    
    renderer1->loadActorData();
    renderer1->loadMapData();
    renderer1->loadSkyBoxData();
    
    screen->print("Flipping the pages...");
    glfwPollEvents();
    glfwSwapBuffers(window);
    
    stbi_set_flip_vertically_on_load(1);

    printf("%s\n", glGetString(GL_VERSION));
    
    renderer0->setUI(ui);
    renderer1->setUI(ui);
    
    std::shared_ptr<HealthMeter> hm =  std::make_shared<HealthMeter>();
    pHero0->getComponent<LifeComponent>()->addObserver(hm);
    ui->insertNode(hm);

    activeRenderer = renderer0;
    activeWorld = &world0; 
    inputHandler.setActiveHero(pHero0);
    activeHero = pHero0;

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
    
    mist.init(3.0, glm::vec3(0,1.0,0), glm::vec3(15, 0.3, 15), 900, 9, 1000, 0.99);
     inkGlyphs.init(0.008, glm::vec3(0,-0.5,0), glm::vec3(0.5, 0.5, 0.5), 2810, 700, 2, 0.995);
    
     fireworks.init(0.2, glm::vec3(0,27,0), glm::vec3(0,0,0), 300, 20, 1000, 0.995);
    fireworks.setColor(glm::vec4(0.3,1.2,3.0,1.0));
     realMap.init(glm::vec3(0,-1.0,0));
     map.init(glm::vec3(0,-0.14,0));

}
                   
void Game::linkObjects() {      
    renderer0->setWorld(&world0);
    renderer1->setWorld(&world1);
                 
    DirectionalLight  dl2(glm::vec3(0.2,0.2,0.2),glm::vec3(0.8,0.8,0.8),glm::vec3(1.0,1.0,1.0),glm::vec3(-1,-1,0));
    world1.setWeather(dl2, 0);  
         
    DirectionalLight   dl(glm::vec3(0.03,0.03,0.03),glm::vec3(0.08,0.08,0.08),glm::vec3(0.5,0.5,0.5),glm::vec3(-1,-1,0));
    world0.setWeather(dl, 0);

    renderer1->setCamera(camera.get());
    renderer0->setCamera(camera.get());
 
    world0.insertCamera(camera.get());
        
    world0.insertParticleEffect(&fireworks);
    world0.insertParticleEffect(&mist);

    world0.insertParticleEffect(&inkGlyphs);
    world0.setMap(map);
    world1.setMap(realMap);
 
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

    world0.tick();

    world1.tick();
    
    activeRenderer->render();
    ui->renderAll(activeRenderer);
    activeRenderer->render2(); 

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
