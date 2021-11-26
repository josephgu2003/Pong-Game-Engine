//
//  MyActorFactory.cpp
//  Pong
//
//  Created by Joseph Gu on 10/30/21.
//

#include "MyActorFactory.hpp"
#include <stdio.h>
#include <string>
#include "AssetManager.hpp"
#include "PhysicsComponent.hpp"
#include "CombatComponent.hpp"
#include "AnimComponent.hpp"
#include "NameComponent.hpp"
#include "LifeComponent.hpp"
#include "AIComponent.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "MeshComponent.hpp"
#include <functional>

typedef std::shared_ptr<Component> ActComp;

MyActorFactory::MyActorFactory() {
    
}

std::shared_ptr<Actor> MyActorFactory::makeActor(int i) {
    std::shared_ptr<Actor> actor = std::make_shared<Actor>();
    actor->dummy = false;
    switch (i) {
        case ACTOR_HOODY: {
            actor->Componentable::addComp<CharacterComponent>(*(actor.get()));
                 
            actor->Componentable::addComp<LifeComponent>(*(actor.get()));
                
            actor->Componentable::addComp<PhysicsComponent>(*(actor.get()), true);
            actor->Componentable::addComp<CombatComponent>(*(actor.get()));
            
            Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
            shader->use();
            shader->setFloat("size", 0.005);
            shader->setFloat("brightness", 0.0);
                 
            Renderer::bindShaderUniblock(shader, ViewProj); 
            Renderer::bindShaderUniblock(shader, Lights);
            Material map;
            AssetManager::loadTexture("Resources/Models/textures/lambert1_baseColor.png", &map.diffuse, true);
            AssetManager::loadTexture("Resources/Models/tmpugfolmqr", &map.normMap, false);
            ActComp gc = std::make_shared<GraphicsComponent>(*(actor.get()), shader, map);
            static_pointer_cast<GraphicsComponent>(gc)->initModel(MOD_HOODY);
            actor->addComp(gc);
            
            break;
        }
        case ACTOR_EVIL_HOODY: {
            actor->Componentable::addComp<CharacterComponent>(*(actor.get()));
            actor->Componentable::addComp<LifeComponent>(*(actor.get()));
            
            actor->setState(STATE_FLYING);
 
            Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
            shader->use();
            shader->setFloat("size", 0.005);
            shader->setFloat("brightness", 0.0);
            
            Renderer::bindShaderUniblock(shader, ViewProj);
            Renderer::bindShaderUniblock(shader, Lights);

            Material map;
            AssetManager::loadTexture(TEX_INKPAPER, &map.diffuse, true);
            AssetManager::loadTexture("Resources/Map/Screen Shot 2021-07-20 at 9.15.42 AM.png", &map.normMap, false);
             
            ActComp gc = std::make_shared<GraphicsComponent>(*(actor.get()), shader, map);
            static_pointer_cast<GraphicsComponent>(gc)->initModel(MOD_HOODY);
            actor->addComp(gc);

            actor->Componentable::addComp<CombatComponent>(*(actor.get()));
            actor->Componentable::addComp<PhysicsComponent>(*(actor.get()), false);
            ActComp ac = std::make_shared<AIComponent>(*(actor.get()));
            actor->addComp(ac);
            break;
        }
        
        case ACTOR_BIRD: {
            actor->Componentable::addComp<LifeComponent>(*(actor.get()));
            actor->setState(STATE_FLYING);
     
            Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
            shader->use();
            shader->setFloat("size", 0.002);
            shader->setFloat("brightness", 0.0);
            Renderer::bindShaderUniblock(shader, ViewProj);

            Renderer::bindShaderUniblock(shader, Lights);
             
            Material map;
            AssetManager::loadTexture("Resources/Models/bird/Tex_Ride_FengHuang_01a_D_A.tga.png", &map.diffuse, true);
            AssetManager::loadTexture("Resources/Map/Screen Shot 2021-07-20 at 9.15.42 AM.png", &map.normMap, false);
      
            ActComp gc = std::make_shared<GraphicsComponent>(*(actor.get()), shader, map);
            static_pointer_cast<GraphicsComponent>(gc)->initModel(MOD_BIRD);
            actor->addComp(gc);
            
            actor->Componentable::addComp<CombatComponent>(*(actor.get()));
            actor->Componentable::addComp<PhysicsComponent>(*(actor.get()), true);
            ActComp ac = std::make_shared<AnimComponent>(*(actor.get()), MOD_BIRD);
            static_pointer_cast<AnimComponent>(ac)->setDefaultAnim("Take 001");
            actor->addComp(ac);
            break;
 
        }
        case ACTOR_VAMP : {
            actor->Componentable::addComp<CharacterComponent>(*(actor.get()));
            actor->Componentable::addComp<LifeComponent>(*(actor.get()));
            
            actor->setState(STATE_FLYING);
            ActComp ac = std::make_shared<AnimComponent>(*(actor.get()), MOD_VAMP);

            static_pointer_cast<AnimComponent>(ac)->setDefaultAnim("");
            actor->addComp(ac);
          //  model = loadModels("Resources/Map/snow3.obj");
            Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
            shader->use();
            shader->setFloat("size", 0.005);
            shader->setFloat("brightness", 0.0);
            
            Renderer::bindShaderUniblock(shader, ViewProj);
            Renderer::bindShaderUniblock(shader, Lights);
            
            Material map;
            AssetManager::loadTexture("Resources/Models/Vampire/Vampire_diffuse.png", &map.diffuse, true);
            AssetManager::loadTexture("Resources/Map/Screen Shot 2021-07-20 at 9.15.42 AM.png", &map.normMap, false);
      
            actor->Componentable::addComp<CombatComponent>(*(actor.get()));
            actor->Componentable::addComp<PhysicsComponent>(*(actor.get()), true);
            

            ActComp gc = std::make_shared<GraphicsComponent>(*(actor.get()), shader, map);
            static_pointer_cast<GraphicsComponent>(gc)->initModel(MOD_VAMP);
            actor->addComp(gc);
            break;

        }
        case ACTOR_KNIGHT : { 
            actor->Componentable::addComp<NameComponent>(*(actor.get()));
            actor->Componentable::addComp<LifeComponent>(*(actor.get()));
        
            Material map;  
            AssetManager::loadTexture("Resources/Models/Knight/hollowknight.png", &map.diffuse, true);
            
            AssetManager::loadTexture(TEX_BLANK_NORMALS, &map.normMap, false);
 
            Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
            shader->use();
            shader->setFloat("size", 0.003);
            shader->setFloat("brightness", 0.0);
              
            Renderer::bindShaderUniblock(shader,      ViewProj);
            Renderer::bindShaderUniblock(shader,      Lights);
              
            actor->Componentable::addComp<PhysicsComponent>(*(actor.get()), true);
            actor->Componentable::addComp<CombatComponent>(*(actor.get()));
            
            ActComp gc = std::make_shared<GraphicsComponent>(*(actor.get()), shader, map);
            static_pointer_cast<GraphicsComponent>(gc)->initModel( "Resources/Models/Knight/hollowknight.fbx");
            actor->addComp(gc);
            ActComp ac = std::make_shared<AnimComponent>(*(actor.get()),  "Resources/Models/Knight/hollowknight.fbx"); 
   
            static_pointer_cast<AnimComponent>(ac)->setDefaultAnim("Armature|Passive");
            actor->addComp(ac);
            break;
            
        }
        case ACTOR_FISH: { // maybe actor would want to know about ability field variables??
            Material map;
            AssetManager::loadTexture(TEX_FISH, &map.diffuse, true); 

            Shader* shader = new Shader("Shaders/SketchVShader.vs", "Shaders/SketchFShader.fs");
            shader->use();
            shader->setFloat("alpha", 1.0);
            
            Renderer::bindShaderUniblock(shader,      ViewProj);
            Renderer::bindShaderUniblock(shader,      Lights);
            Renderer::bindShaderUniblock(shader,      StopWatch);
             
            ActComp gc = std::make_shared<GraphicsComponent>(*(actor.get()), shader, map);
            VertexMesh* mesh = new VertexMesh;
            int gridX = 20;
            int gridY = 3;
            float length = 1.5;
            static_pointer_cast<GraphicsComponent>(gc)->initGrid(gridX, gridY, length, mesh);
            actor->addComp(gc); 
                
            ActComp mc = std::make_shared<MeshComponent>((*(actor.get())));
            static_pointer_cast<MeshComponent>(mc)->setMesh(mesh);
         
            std::shared_ptr<float> lastYaw = std::make_shared<float>(0.0f);
            std::shared_ptr<bool> first = std::make_shared<bool>(true);
                
            std::function<void (VertexMesh*)> meshAction = [=] (VertexMesh* v) { // idiot code to avoid making custom class lmao
                if (*(first.get())) {
                    *lastYaw.get() = actor->getYaw();
          
                    *first.get() = false;  
                    return;
                }
                 
                float deltayaw = actor->getYaw() - *lastYaw.get();
                deltayaw *= -1; //reverse
                deltayaw = glm::radians(deltayaw);
                *lastYaw.get() = actor->getYaw();

                
                for (int i = 0; i < gridX; i++) { // loop over columns
                    for (int j = 0; j < gridY; j++) { // loop through a column
                        if (i==0) {
                            //first column will always move with the actor pos center
                        } else {
                        glm::vec3& lastVertex = v->vertices.at(i+j*gridX).Pos;
                //            lastVertex += glm::vec3(length*0.5,0,0);
                            lastVertex.x = lastVertex.x * cos(deltayaw) - lastVertex.z*sin(deltayaw);
                            lastVertex.z = lastVertex.x * sin(deltayaw) + lastVertex.z*cos(deltayaw);
                            // change of basis
                            lastVertex += glm::vec3(0.08,0,0);
                      //      lastVertex -= glm::vec3(length*0.5,0,0);
                        glm::vec3 dir = v->vertices.at((i-1)+j*gridX).Pos - lastVertex;
                        float lengthDir = glm::length(dir);
                        float p = 1.0f - (length/(gridX-1))/lengthDir;
                        lastVertex += p * dir;
                        }
                    }
            }
                  
                
            }; 

            static_pointer_cast<MeshComponent>(mc)->setMeshAction(meshAction);
            actor->addComp(mc);
            actor->offsetOrientationVectors(glm::vec3(0,90,0));
            break; 
        }
        default: {
               
        Material map;
        AssetManager::loadTexture("Resources/Map/8grass.png", &map.diffuse, true);
            
        Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
        shader->use();
        shader->setFloat("size", 0.1);
        shader->setFloat("brightness", 0.0);
            
            Renderer::bindShaderUniblock(shader,      ViewProj);

            Renderer::bindShaderUniblock(shader,      Lights);
            
            actor->Componentable::addComp<PhysicsComponent>(*(actor.get()), true);
            
            ActComp gc = std::make_shared<GraphicsComponent>(*(actor.get()), shader, map);
            static_pointer_cast<GraphicsComponent>(gc)->initModel("Resources/Map/grass1.fbx");
            actor->addComp(gc);
            actor->rotate(glm::vec3(-1.5,0,0));
            break;
        }
}
    return actor;
}
