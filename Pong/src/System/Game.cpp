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

 
extern void char_callback(GLFWwindow* window, unsigned int key);
extern void onetap_callback0(GLFWwindow* window, int key, int scancode, int action, int mods);
extern void mouse_callback(GLFWwindow* window, double mouseX_, double mouseY_);

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

Game::Game() {
    t0 = std::chrono::high_resolution_clock::now();
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

    //billow.posVec = glm::vec3(0,5,0);
    stbi_set_flip_vertically_on_load(0);
    
    initObjects();
  
    linkObjects();
    
    JsonManager::loadGame(this);
    
    inputHandler.setPlayerHero(pHero0, 0);
    inputHandler.setPlayerHero(pHero1, 1);
    
    inkGlyphs.setActor(pHero0.get());
    camera->setActor(pHero0.get());
    
    VertexData* data = new VertexData;
    std::shared_ptr<TBNVertex> a = std::make_shared<TBNVertex>(glm::vec3(-50,0,-50),glm::vec3(0,1,0),glm::vec2(0,0), glm::vec3(0,0,0), glm::vec3(0,0,0));
    std::shared_ptr<TBNVertex> b = std::make_shared<TBNVertex>(glm::vec3(50,0,-50),glm::vec3(0,1,0),glm::vec2(1,0), glm::vec3(0,0,0), glm::vec3(0,0,0));
    std::shared_ptr<TBNVertex> c = std::make_shared<TBNVertex>(glm::vec3(50,0,50),glm::vec3(0,1,0),glm::vec2(1,1), glm::vec3(0,0,0), glm::vec3(0,0,0));
    std::shared_ptr<TBNVertex> d = std::make_shared<TBNVertex>(glm::vec3(-50,0,50),glm::vec3(0,1,0),glm::vec2(0,1), glm::vec3(0,0,0), glm::vec3(0,0,0));
    std::vector<std::shared_ptr<AnyVertex>> mapVertices = {a, b, c, d
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
     
    AssetManager::bindShaderUniblock(shader, AssetManager::ViewProj);
    AssetManager::bindShaderUniblock(shader, AssetManager::Lights);
    AssetManager::bindShaderUniblock(shader, AssetManager::StopWatch);
 
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

    AssetManager::bindShaderUniblock(shader2, AssetManager::ViewProj);
    AssetManager::bindShaderUniblock(shader2, AssetManager::Lights);
    AssetManager::bindShaderUniblock(shader2, AssetManager::StopWatch);
  //  world.insertActor(&billow)
 

    screen->print("Preparing the brushes...");
    glfwPollEvents(); 
    glfwSwapBuffers(window);

    
    screen->print("Opening the books...");
    glfwPollEvents();
    glfwSwapBuffers(window);

    
    screen->print("Flipping the pages...");
    glfwPollEvents();
    glfwSwapBuffers(window);

    screen->print("Putting on a fresh canvas...");
    glfwPollEvents();
    glfwSwapBuffers(window);
    
    renderer0->loadActorData();
    renderer0->loadMapData();
    renderer0->loadSkyBoxData();
    
    renderer1->loadActorData();
    renderer1->loadMapData();
    renderer1->loadSkyBoxData();
    
    stbi_set_flip_vertically_on_load(1);

    printf("%s\n", glGetString(GL_VERSION));

    int i;
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &i);
    printf("%i\n", i);

    activeRenderer = renderer0;
    activeWorld = &world0;

    script = new ScriptOne();
    script->init(this);
  //  static_cast<AnimComponent*>(tree->getComp(ANIM).get())->playAnim("");
 //   tree->getComponent<AnimComponent>()->playAnim("");
}

Game::~Game() {
     
}   

void Game::tick() {    

    auto t1 = std::chrono::high_resolution_clock::now();
 
        /* Getting number of milliseconds as an integer. */
        auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0);

    int t = ms_int.count();
    float ratio = (float)t /30.0f; 
    if (floor(ratio) == intervalTimer) {
        intervalTimer = floor(ratio);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        return;
    }
    else {
        intervalTimer = floor(ratio);

    }
    
    glfwSetTime(glfwGetTime()-lastTime);
    lastTime = glfwGetTime();
 //   printf("Delta t is %f\n", (float)lastTime );
    fpsTimer += lastTime;
    
    
    
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
    
  /**  if(ball->getComponent<CombatComponent>()->QHasAbilities()) {
        std::vector<std::shared_ptr<Ability>>& q = ball->getComponent<CombatComponent>()->getAbilityQ();
        for(int i = 0; i < q.size(); i++) {
            q.at(i)->call(this);
            abilities.push_back(q.at(i));
        }
        q.clear();
    }**/
    
  /**  if(pHero0->getComponent<CombatComponent>()->QHasAbilities()) {
        std::vector<std::shared_ptr<Ability>>& q = pHero0->getComponent<CombatComponent>()->getAbilityQ();
        for(int i = 0; i < q.size(); i++) {
            q.at(i)->call(this);
            abilities.push_back(q.at(i));
        }
        q.clear();
    }
    
    if(pHero1->getComponent<CombatComponent>()->QHasAbilities()) {
        std::vector<std::shared_ptr<Ability>>& q = pHero1->getComponent<CombatComponent>()->getAbilityQ();
        for(int i = 0; i < q.size(); i++) {
          q.at(i)->call(this);
            abilities.push_back(q.at(i));
        }
        q.clear();
    }
    
    if (abilities.size() > 0) {
        for(int i = 0; i < abilities.size(); i++) {
            if(abilities.at(i)->on == true) {
                abilities.at(i)->tick();
            }
            
    }
         
    }**/

  script->tick();

    world0.tick();

    world1.tick();

  

    
    activeRenderer->render();

    glfwPollEvents();
        glfwSwapBuffers(window);
}



/**Numberable* Game::getNumberable(unsigned int ID_) {
    return numberables[ID_];
}**/
 
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
/**void Game::setNumberable(Numberable* numberable, int i) {
    numberables[i] = numberable;
} 
**/

InputHandler& Game::getInputHandler() {
    return inputHandler;
}

void Game::swapWorld() {
    if (activeWorld == &world0) {
        inputHandler.setActiveHero(1);
   
        activeWorld = &world1;
        activeRenderer = renderer1;
        activeRenderer->updateLights();
        camera->setActor(pHero1.get());
        return;
    }
    if (activeWorld == &world1) {
        inputHandler.setActiveHero(0);
        activeWorld = &world0;
        activeRenderer = renderer0;
        activeRenderer->updateLights();
        camera->setActor(pHero0.get());
        return;
    }
}
  
