//
//  Actor.cpp
//  Pong
//
//  Created by Joseph Gu on 5/4/21.
//

#include "Actor.hpp"
#include "World.hpp"
#include <stdio.h>
#include <string>
#include <iostream>
#include "AssetManager.hpp"
#include "Ability.hpp"
#include "PhysicsComponent.hpp"
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "CombatComponent.hpp"
#include "AnimComponent.hpp"
#include "NameComponent.hpp"  
#include "LifeComponent.hpp"
#include "CharacterComponent.hpp"
#include "Renderer.hpp"
#include "AIComponent.hpp"
 
#define JUMP_SPEED 0.05f

Actor::Actor() {
    state = STATE_IDLE;
        posVec.x = 0.0f;
        posVec.y = 0.0f;
        posVec.z = 0.0f;
        dirVec = glm::vec3(0.0f, 0.0f, -1.0f);
        velVec = glm::vec3(0.0f, 0.0f, 0.0f);
        rightVec = glm::cross(dirVec,glm::vec3(0,1,0));
    eulerAngles = glm::vec3(0,0,0);
    distribution = std::uniform_int_distribution<int>(1,1000);
} 

Actor::~Actor() {
 
} 

void Actor::orient(float yaw_) {
    eulerAngles.y = yaw_;
    dirVec.x = std::cos(glm::radians(eulerAngles.y));
    dirVec.z = std::sin(glm::radians(eulerAngles.y));
    rightVec = rightVec = glm::cross(dirVec,glm::vec3(0,1,0));
    dirVec = glm::normalize(dirVec);
    rightVec = glm::normalize(rightVec);
} 

void Actor::turnTowards(const glm::vec3& newDir_) {
    glm::vec3 newDir = glm::normalize(newDir_);

    float dYaw = (180.0f/3.14159)* glm::orientedAngle(glm::normalize(glm::vec2(newDir.x,newDir.z)), glm::normalize(glm::vec2(dirVec.x,dirVec.z)));

    eulerAngles -= glm::vec3(0,dYaw,0);
    if (eulerAngles.x < -45.0f) {
        eulerAngles.x = -45.0f;
    }

    dirVec = newDir;
    rightVec = rightVec = glm::cross(dirVec,glm::vec3(0,1,0));
    rightVec = glm::normalize(rightVec);
}

World& Actor::getWorld() {
    return *world;
}


void Actor::tick() {
    
    Componentable::tick();
    
    
    
    modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, posVec);
    glm::vec3 rotations = glm::vec3(eulerAngles.x,glm::radians(90.0f-eulerAngles.y),glm::radians(eulerAngles.z));
    glm::quat MyQuaternion = glm::quat(rotations);
    
    glm::mat4 RotationMatrix = toMat4(MyQuaternion);
    modelMat = modelMat * RotationMatrix;
    
    graphics->getShader()->setMat4("modelMat", modelMat);
    
    glm::mat3 transposeInverse = glm::mat3(glm::transpose(glm::inverse(modelMat)));
    graphics->getShader()->setMat3("transposeInverseModelMat", transposeInverse);
}

void Actor::setPos(glm::vec3 pos_) {
    posVec = pos_;
}

void Actor::translatePos(const glm::vec3& translate) {
    posVec += translate;
}

void Actor::setPosY(float y_) {
    posVec.y = y_;
}
void Actor::posDir(float speed) {
    if (state != STATE_PARALYZED) {
    posVec += speed * dirVec;
    }
}

void Actor::posRight(float speed) {
    if (state != STATE_PARALYZED) {
    posVec += speed * rightVec;
    }
}

void Actor::randomPosAround(glm::vec3 pivot) {

}



void Actor::jump() {
    if (state != STATE_PARALYZED) {
    velVec.y = JUMP_SPEED;
    }
}

void Actor::velRight(float speed) {
    velVec += rightVec*speed;
}
void Actor::velDir(float speed) {
    velVec += dirVec*speed;
}

glm::vec3 Actor::getVel() {
    return velVec;
}

void Actor::setVel(glm::vec3 velVel_) {
    velVec = velVel_;
}

void Actor::accelerate(glm::vec3 accel) {
    velVec += accel;
}

float Actor::getYaw() {
    return eulerAngles.y;
}

glm::vec3 Actor::getPos() {
    return posVec;  
}

void Actor::init(int i ) {
    dummy = false;
    if (i == 0) {
        std::shared_ptr<Component> nc = std::make_shared<CharacterComponent>(*this);
        addComp(nc);
            
        std::shared_ptr<Component> lc = std::make_shared<LifeComponent>(*this);
        addComp(lc);

        Model* model = new Model();
        AssetManager::loadModel(MOD_HOODY, model);

        TextureMaps map;
        AssetManager::loadTexture("Resources/Models/textures/lambert1_baseColor.png", &map.diffuse, true);
        AssetManager::loadTexture("Resources/Models/tmpugfolmqr", &map.normMap, false);
            
        for (int i = 0; i<model->getMeshes()->size(); i ++) {
            model->setMeshTexture(i, map);
        }
            
        Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
        shader->use();
        shader->setFloat("size", 0.005);
        shader->setFloat("brightness", 3.0);
            
        Renderer::bindShaderUniblock(shader, ViewProj);
        Renderer::bindShaderUniblock(shader, Lights);
            
        std::shared_ptr<Component> pc = std::make_shared<PhysicsComponent>(*this, true);
        addComp(pc);
        std::shared_ptr<Component> cc = std::make_shared<CombatComponent>(*this);
            addComp(cc);
        std::shared_ptr<Component> gc = std::make_shared<GraphicsComponent>(shader);
        static_pointer_cast<GraphicsComponent>(gc)->setModel(model);
        addComp(gc);
        
    }
    if (i == 1) {
        std::shared_ptr<Component> nc = std::make_shared<CharacterComponent>(*this);
        addComp(nc);
        std::shared_ptr<Component> lc = std::make_shared<LifeComponent>(*this);
        addComp(lc);
        
        state = STATE_FLYING;
        Model* model = new Model();
        AssetManager::loadModel(MOD_HOODY, model); 

        Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
        shader->use();
        shader->setFloat("size", 0.005); 
        shader->setFloat("brightness", 1.0);
        
        Renderer::bindShaderUniblock(shader, ViewProj);
        Renderer::bindShaderUniblock(shader, Lights);

        TextureMaps map;
        AssetManager::loadTexture(TEX_INKPAPER, &map.diffuse, true);
        AssetManager::loadTexture("Resources/Map/Screen Shot 2021-07-20 at 9.15.42 AM.png", &map.normMap, false);
        for (int i = 0; i<model->getMeshes()->size(); i ++) {
            model->setMeshTexture(i, map);
        }

        std::shared_ptr<Component> cc = std::make_shared<CombatComponent>(*this);
        addComp(cc);
        std::shared_ptr<Component> pc = std::make_shared<PhysicsComponent>(*this, false);
        addComp(pc);

        std::shared_ptr<Component> gc = std::make_shared<GraphicsComponent>(shader);
        static_pointer_cast<GraphicsComponent>(gc)->setModel(model);
        addComp(gc);

    }
    if (i == 2) {
        std::shared_ptr<Component> nc = std::make_shared<NameComponent>(*this);
        addComp(nc);
        std::shared_ptr<Component> lc = std::make_shared<LifeComponent>(*this);
        addComp(lc);
        
        state = STATE_FLYING;
        std::shared_ptr<Component> ac = std::make_shared<AnimComponent>(*this);


        Model* model = new Model;
        AssetManager::loadModel(MOD_BIRD, model,static_cast<AnimComponent*>(ac.get()));
        static_pointer_cast<AnimComponent>(ac)->setDefaultAnim("Take 001");
        addComp(ac);

        Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
        shader->use();
        shader->setFloat("size", 0.005);
        shader->setFloat("brightness", 1.0);
        Renderer::bindShaderUniblock(shader, ViewProj);

        Renderer::bindShaderUniblock(shader, Lights);
        
        TextureMaps map;
        AssetManager::loadTexture("Resources/Models/bird/Tex_Ride_FengHuang_01a_D_A.tga.png", &map.diffuse, true); 
        AssetManager::loadTexture("Resources/Map/Screen Shot 2021-07-20 at 9.15.42 AM.png", &map.normMap, false);
        for (int i = 0; i<model->getMeshes()->size(); i ++) {
            model->setMeshTexture(i, map);
        }
  
        std::shared_ptr<Component> cc = std::make_shared<CombatComponent>(*this);
        addComp(cc);
        std::shared_ptr<Component> pc = std::make_shared<PhysicsComponent>(*this, false);
        addComp(pc);

        std::shared_ptr<Component> gc = std::make_shared<GraphicsComponent>(shader);
        static_pointer_cast<GraphicsComponent>(gc)->setModel(model);
        addComp(gc);

    }
    if (i == 3) {
        std::shared_ptr<Component> nc = std::make_shared<CharacterComponent>(*this);
        addComp(nc);
        std::shared_ptr<Component> lc = std::make_shared<LifeComponent>(*this);
        addComp(lc);
        
        state = STATE_FLYING;
        std::shared_ptr<Component> ac = std::make_shared<AnimComponent>(*this);

        Model* model = new Model(MOD_VAMP, static_cast<AnimComponent*>(ac.get()));
        static_pointer_cast<AnimComponent>(ac)->setDefaultAnim("");
        addComp(ac); 
      //  model = loadModels("Resources/Map/snow3.obj");
        Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
        shader->use();
        shader->setFloat("size", 0.005);
        shader->setFloat("brightness", 1.0);
        
        Renderer::bindShaderUniblock(shader, ViewProj);
        Renderer::bindShaderUniblock(shader, Lights);
        
        TextureMaps map;
        AssetManager::loadTexture("Resources/Models/Vampire/Vampire_diffuse.png", &map.diffuse, true);
        AssetManager::loadTexture("Resources/Map/Screen Shot 2021-07-20 at 9.15.42 AM.png", &map.normMap, false);
        for (int i = 0; i<model->getMeshes()->size(); i ++) {
            model->setMeshTexture(i, map);
        }

        std::shared_ptr<Component> cc = std::make_shared<CombatComponent>(*this);
        addComp(cc);
        std::shared_ptr<Component> pc = std::make_shared<PhysicsComponent>(*this, false);
        addComp(pc);
        

        std::shared_ptr<Component> gc = std::make_shared<GraphicsComponent>(shader);
        static_pointer_cast<GraphicsComponent>(gc)->setModel(model);
        addComp(gc);

    }
    if (i == 4) {
        std::shared_ptr<Component> aic = std::make_shared<AIComponent>(*this);
        addComp(aic);
        std::shared_ptr<Component> nc = std::make_shared<CharacterComponent>(*this);
        addComp(nc);
        std::shared_ptr<Component> lc = std::make_shared<LifeComponent>(*this);
        addComp(lc);
        std::shared_ptr<Component> ac = std::make_shared<AnimComponent>(*this);

    Model* model = new Model();

    AssetManager::loadModel("Resources/Models/Knight/knight.fbx", model,  static_cast<AnimComponent*>(ac.get()));
         
    static_pointer_cast<AnimComponent>(ac)->setDefaultAnim("HollowKnight__Armature|Walk");
    addComp(ac);  
          
    TextureMaps map;
    AssetManager::loadTexture("/Users/josephgu/Downloads/hollow_knight_-_vr_chat_-_free_download/textures/Knight_baseColor.png", &map.diffuse, true);
        
    AssetManager::loadTexture("Resources/Models/tmpugfolmqr", &map.normMap, false);
        
    for (int i = 0; i<model->getMeshes()->size(); i ++) {
        model->setMeshTexture(i, map);
    }
          
    Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
    shader->use();
    shader->setFloat("size", 0.007);
    shader->setFloat("brightness", 3.0);
        
        Renderer::bindShaderUniblock(shader,      ViewProj);

        Renderer::bindShaderUniblock(shader,      Lights);
        
        std::shared_ptr<Component> pc = std::make_shared<PhysicsComponent>(*this, true);
        addComp(pc);
        std::shared_ptr<Component> cc = std::make_shared<CombatComponent>(*this);
        addComp(cc);
        
        std::shared_ptr<Component> gc = std::make_shared<GraphicsComponent>(shader);
        static_pointer_cast<GraphicsComponent>(gc)->setModel(model);
        addComp(gc);
        
    }
}

void Actor::setWorld(World* world_) {
    world = world_;
    Componentable::init(this, world);
}

void Actor::setState(State state_) {
    state = state_;
}
State Actor::getState() {
    return state; 
}

void Actor::addComp(const std::shared_ptr<Component>& comp) {
    Componentable::addComp(comp);
    if (comp->getType() == GRAPHICS) graphics = static_pointer_cast<GraphicsComponent>(comp);
}
  
float Actor::getDistanceTo(Actor* b) {
    float d = glm::length(posVec - b->getPos());
    return d;
}
