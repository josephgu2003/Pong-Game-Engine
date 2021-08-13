//
//  Game.cpp
//  Pong
//
//  Created by Joseph Gu on 6/3/21.
//

#include "Game.hpp"
#include "FallingLetters.hpp"
#include <iostream>
#include <cctype>
#include "Speech.hpp"
#include "stb_image.h"
#include "Dialogue.hpp"
#include "Fish.hpp"
#include "Ability.hpp"
#include <thread>
#include "json.hpp"
#include "AssetManager.hpp"
#include "CombatComponent.hpp"
#include "AnimComponent.hpp"
 
extern void char_callback(GLFWwindow* window, unsigned int key);
extern void onetap_callback0(GLFWwindow* window, int key, int scancode, int action, int mods);
extern void mouse_callback(GLFWwindow* window, double mouseX_, double mouseY_);

void Game::initWindow() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    window = glfwCreateWindow(1000, 650, "OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
}
  
void Game::initObjects() {
    ball = std::make_shared<Actor>();
    pHero = std::make_shared<Actor>(); 
    rHero = std::make_shared<Actor>();
    tree = std::make_shared<Actor>();
    realRenderer = new Renderer; 
     renderer = new Renderer;
    mist.init(3.0, glm::vec3(0,1.0,0), glm::vec3(15, 0.3, 15), 900, 9, 1000, 0.99);
     inkGlyphs.init(0.008, glm::vec3(0,-0.5,0), glm::vec3(0.5, 0.5, 0.5), 2810, 700, 2, 0.995);
    
     fireworks.init(0.2, glm::vec3(0,27,0), glm::vec3(0,0,0), 300, 20, 1000, 0.995);
    fireworks.setColor(glm::vec4(0.3,1.2,3.0,1.0));
     realMap.init(glm::vec3(0,-1.0,0));
     map.init(glm::vec3(0,-0.14,0));
     ball->init(2);
     ball->setID(1);  
     numberables[1] = ball.get(); 
    
     pHero->init(0);
     pHero->setID(2);
     numberables[2] = pHero.get();
        
     pHero->setPos(glm::vec3(10,35,10)); 
     ball->setPos(glm::vec3(0,1.7,0));
    // billow.loadModel();
     rHero->init(0);
    tree->setPos(glm::vec3(0,0.7,0));  
    tree->init(3);
     
     world.setID(0);
     numberables[0] = &world;
    
}
                   
void Game::linkObjects() {      
    renderer->setWorld(&world);
    realRenderer->setWorld(&realWorld);
                 
    DirectionalLight  dl2(glm::vec3(0.2,0.2,0.2),glm::vec3(0.8,0.8,0.8),glm::vec3(1.0,1.0,1.0),glm::vec3(-1,-1,0));
    realWorld.setWeather(dl2, 0);  
         
    DirectionalLight   dl(glm::vec3(0.03,0.03,0.03),glm::vec3(0.08,0.08,0.08),glm::vec3(0.5,0.5,0.5),glm::vec3(-1,-1,0));
    world.setWeather(dl, 0);
    inkGlyphs.setActor(pHero.get());
    camera.setActor(pHero.get());
    realRenderer->setCamera(&camera);  
    renderer->setCamera(&camera);
 
    world.insertCamera(&camera);
    world.insertActor(pHero);
    world.insertActor(ball);
    realWorld.insertActor(tree);
    realWorld.insertActor(rHero);
       
     world.insertParticleEffect(&fireworks);   
     world.insertParticleEffect(&mist);

       world.insertParticleEffect(&inkGlyphs);
       world.setMap(map);
       realWorld.setMap(realMap);
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
    
    glfwSetWindowUserPointer(window, this);
    
    screen = new LoadingScreen();
    
    inputHandler.setWindow(window);
    
    //billow.posVec = glm::vec3(0,5,0);
    stbi_set_flip_vertically_on_load(0);
    
    initObjects();
  
    linkObjects();

    
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
 
    Model*  model = loadModels("Resources/Map/snow3.obj");
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
    
    renderer->loadActorData();
    renderer->loadMapData();
    renderer->loadSkyBoxData();
    
    realRenderer->loadActorData();
    realRenderer->loadMapData();
    realRenderer->loadSkyBoxData();
    
    stbi_set_flip_vertically_on_load(1);

    printf("%s\n", glGetString(GL_VERSION));

    int i;
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &i);
    printf("%i\n", i);
    
    blank = stbi_load("Resources/Particles/BLANK_ICON3.png", &imageWidth, &imageHeight, &channels, 0);
     
    glGenTextures(1, &ftexture);
    glBindTexture(GL_TEXTURE_2D, ftexture);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,blank);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
     glBindTexture(GL_TEXTURE_2D, 0);

    paint = stbi_load("Resources/Particles/pencil.jpg", &imageWidth, &imageHeight, &channels, 0);
  //  audio.playMusic();
    activeRenderer = renderer;

    script = new ScriptOne();
    script->init(this);
    static_cast<AnimComponent*>(tree->getComp(ANIM).get())->playAnim("");
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
        int k = 1;
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

    if (scheme == 0) { 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        pHero->posDir(0.03);
        rHero->posDir(0.03);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        pHero->posDir(-0.03);
        rHero->posDir(-0.03);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        pHero->posRight(0.03);
        rHero->posRight(0.03);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        pHero->posRight(-0.12);
        rHero->posRight(-0.03);
    }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            activeRenderer->incExposure(0.01);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            activeRenderer->incExposure(-0.01);
        }
    }
     
    inputHandler.tick();
  
    double mx, my;
    
    if (scheme == 2)     {
        glfwGetCursorPos(window, &mx, &my);
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                //quad and framebuffer
                //write to framebuffer at that position
                for (int j = 0; j < 50; j++) {
           //  glTexSubImage2D(GL_TEXTURE_2D, 0, 800*(1-trail.at(i).x/1000), 800*(1-trail.at(i).y/800),      imageWidth, imageHeight, GL_RGBA, GL_UNSIGNED_BYTE, paint);
                    float ratio1 = (float)(j/50.0f);
                    float ratio2 = 1.0f - ratio1;
                    glBindTexture(GL_TEXTURE_2D, ftexture);
                    //   glTexSubImage2D(GL_TEXTURE_2D, 0, 800*(1-(ratio2*lastMX+ratio1*mx)/1000), 800*(1-((ratio2*lastMY+ratio1*my)/800)), imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, paint);
                    glTexSubImage2D(GL_TEXTURE_2D, 0, 800.0*(1.0-(ratio2*lastMX+ratio1*mx)/1000.0), 800.0*(1.0-((ratio2*lastMY+ratio1*my)/650.0)), imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, paint);
                } 
            }
    lastMY = my;
    lastMX = mx;
    }

    if(glfwWindowShouldClose(window)) {
        running = false;
    }
    
    if((static_pointer_cast<CombatComponent>(ball->getComp(COMBAT))->QHasAbilities())) {
        std::vector<std::shared_ptr<Ability>>& q = static_pointer_cast<CombatComponent>(ball->getComp(COMBAT))->getAbilityQ();
        for(int i = 0; i < q.size(); i++) {
            q.at(i)->call(this);
            abilities.push_back(q.at(i));
        }
        q.clear();
    }
    
    if(static_pointer_cast<CombatComponent>(pHero->getComp(COMBAT))->QHasAbilities()) {
        std::vector<std::shared_ptr<Ability>>& q = static_pointer_cast<CombatComponent>(pHero->getComp(COMBAT))->getAbilityQ();
        for(int i = 0; i < q.size(); i++) {
            q.at(i)->call(this);
            abilities.push_back(q.at(i));
        }
        q.clear();
    }
    
    if(static_pointer_cast<CombatComponent>(rHero->getComp(COMBAT))->QHasAbilities()) {
        std::vector<std::shared_ptr<Ability>>& q = static_pointer_cast<CombatComponent>(rHero->getComp(COMBAT))->getAbilityQ();
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
         
    }

  script->tick();

    world.tick();

    realWorld.tick();

    if (abilities.size() > 0) {
        for(int i = 0; i < abilities.size(); i++) {
            if(abilities.at(i)->on == false) {
                abilities.at(i).reset();
                abilities.erase(abilities.begin()+i);
            }
    }
    }
   // printf("%f\n", glfwGetTime());
    if(printing)
    {print();}
    
    activeRenderer->render();

    glfwPollEvents();
        glfwSwapBuffers(window);
}

void Game::moveMouse(double mouseX_, double mouseY_) {
    if (scheme != 2) {
    if (firstMouse) {
        lastMX = mouseX_;
        lastMY = mouseY_;
        firstMouse = false;
    }
    xOffset = mouseX_ - lastMX;
    yOffset = lastMY - mouseY_;
    lastMX = mouseX_;
    lastMY = mouseY_;
    camera.incYaw(xOffset*0.03);
    camera.incPitch(yOffset*0.03);
    xOffset = 0;
    yOffset = 0;
    }
} 

int Game::processInput(int key, int action, int mods) {
//    unique_lock<mutex> lock()
    if (scheme == 3) {
        if (key == GLFW_KEY_A && action == GLFW_PRESS) {
            nextBranch = 0;
            if(activeDialogue != NULL) activeDialogue->branch(nextBranch);
            setActionScheme(0);
        }
            if (key == GLFW_KEY_B && action == GLFW_PRESS) {
                nextBranch = 1;
                if(activeDialogue != NULL) activeDialogue->branch(nextBranch);
                setActionScheme(0);
            }
    }
    if (scheme == 1) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_SPACE)
        {
          //  input.put(' ');
            i.append(" ");
        }
    const char* key_name = glfwGetKeyName((key), 0);
    if (key_name != NULL)
       // input << (key_name);
        if (mods == GLFW_MOD_SHIFT){
        char c = toupper(*key_name); 
            if (key == GLFW_KEY_1) c = '!';
            if (key == GLFW_KEY_SLASH) c = '?';
            if (key == GLFW_KEY_APOSTROPHE) c = '\"';
        i.append(std::string(1, c));
         } else if (mods != GLFW_MOD_SHIFT) {
            i.append(key_name);  
         }
    } 
        
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
            scheme = 0;
          //  std::string test;
          //  input >> test;
            std::cout << i;
        printing = true;
            return 1;
        }
    }
    
    if (scheme == 0) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        pHero->jump();
        rHero->jump();
    }
    if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        std::shared_ptr<Ability> letters = std::make_shared<FallingLetters>(&world, pHero.get(), 6);

        if (static_pointer_cast<CombatComponent>(pHero->getComp(COMBAT))->hasTarget()) {
            letters->setTarget(static_pointer_cast<CombatComponent>(pHero->getComp(COMBAT))->getBigTarget());
        } 
        letters->call(this);
        abilities.push_back(letters);
    }
        if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
            std::vector<std::string> lines = {"WASD - Move", "don't touch E, R, T - old features that need new purpose", "Z - summon fish and break stuns", "X - swap world", "The boss ahead will dialogue you", "Then stun, press Z after", "Joseph Gu - Programmer", "Yirou Guo - Creative Consultant and Artist", "Jonathan Ran - Mathematical and Physics Consultant", "Matthew Ding - Deployment Help"};
            std::shared_ptr<Ability> speech = std::make_shared<Speech>(&world, pHero.get(), 6.0, lines);
            abilities.push_back(speech);
            speech->call(this);
        }  
        
        if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
            std::shared_ptr<Ability> fish = std::make_shared<Fish>(&world, pHero.get(), 18.0);
            abilities.push_back(fish);
            fish->call(this);
        }
         
        if (key == GLFW_KEY_X && action == GLFW_PRESS) {
            if (activeRenderer == renderer) {
            activeRenderer = realRenderer;
                activeRenderer->updateLights();
            camera.setActor(rHero.get());
                return 0;
            }
            if (activeRenderer == realRenderer) {
            activeRenderer = renderer;
                activeRenderer->updateLights();
            camera.setActor(pHero.get());
                return 0;
            }
        }
        
        if (key == GLFW_KEY_E && action == GLFW_PRESS) {
            int newScheme = (-1)*(scheme-2);
            setActionScheme(newScheme); 
            if (activeSketch.get() == NULL) {
            std::shared_ptr<Ability> sketch = std::make_shared<Sketch>(&world, pHero.get(), 6, ftexture);
            activeSketch = static_pointer_cast<Sketch>(sketch);
            abilities.push_back(sketch);
            sketch->call(this);
            }
            return 0; 
        }    
        
        if (key == GLFW_KEY_R && action == GLFW_PRESS) { 
            glBindTexture(GL_TEXTURE_2D, ftexture);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 800, 800, GL_RGB, GL_UNSIGNED_BYTE, blank);
         //   glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 800, 800, GL_RGBA, GL_UNSIGNED_BYTE, blank);
            glBindTexture(GL_TEXTURE_2D, 0);
            if (activeSketch.get() != NULL) {
            abilities.erase(std::remove(abilities.begin(), abilities.end(), activeSketch));
            activeSketch.reset();
            }
        }  
         
        if (key == GLFW_KEY_T && action == GLFW_PRESS) {
            if (activeSketch.get() != NULL)
            activeSketch->call2();
        }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
    //    inputHandler.setCharCallback(char_callback);
 //       inputHandler.setKeyCallback(onetap_callback0);
        scheme = 1;
    }
    }
    if (scheme == 2) {
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        int newScheme = (-1)*(scheme-2);
        setActionScheme(newScheme);
        firstMouse = true;
    }
    }
    return 0;
}

void Game::processInput2(int key, int action) {

}

void Game::print() {
    activeRenderer->print(i);
    //printf("Exposure is %f \n", activeRenderer->exposure);
    printing = false;
    i = ""; 
}

void Game::setActionScheme(int id) {
    if(id == 2) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(window, mouse_callback);
    }
    if(scheme == 2 && id !=2) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    scheme = id;
}

void Game::newDialogue(Dialogue& dialogue_) {
    activeDialogue = &dialogue_;
}

Numberable* Game::getNumberable(unsigned int ID_) {
    return numberables[ID_];
}
