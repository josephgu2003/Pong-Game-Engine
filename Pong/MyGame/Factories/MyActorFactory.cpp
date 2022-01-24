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
#include "InventoryComponent.hpp"
#include "CollisionComponent.hpp" 
#include "MyGameItems.hpp"
#include "LightComponent.hpp"
 
typedef std::shared_ptr<Component> ActComp;
 
MyActorFactory::MyActorFactory() {
    
}

std::shared_ptr<Actor> MyActorFactory::makeActor(int i) {
    stbi_set_flip_vertically_on_load(0);
    std::shared_ptr<Actor> actor = std::make_shared<Actor>();
    actor->dummy = false;
    switch (i) {
        case ACTOR_HOODY: {
            actor->Componentable::addComponent<CharacterComponent>(*(actor.get()));
            actor->Componentable::addComponent<LifeComponent>(*(actor.get()));
            actor->Componentable::addComponent<PhysicsComponent>(*(actor.get()), true);
            actor->Componentable::addComponent<CombatComponent>(*(actor.get()));
            
            Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
            shader->use();
            shader->setUniform("size", 0.005f);
            shader->setUniform("brightness", 0.0f);
                 
            Renderer::bindShaderUniblock(shader, ViewProj); 
            Renderer::bindShaderUniblock(shader, Lights);
            
            Material map;
            AssetManager::loadTexture("Resources/Models/textures/lambert1_baseColor.png", &map.diffuse, true);
            AssetManager::loadTexture("Resources/Models/tmpugfolmqr", &map.normMap, false);
            ActComp gc = std::make_shared<GraphicsComponent>(*(actor.get()), shader, map, DRAW_OPAQUE);
            static_pointer_cast<GraphicsComponent>(gc)->initModel(MOD_HOODY);
            actor->addComp(gc);
            
            break;
        }
        case ACTOR_EVIL_HOODY: {
            actor->Componentable::addComponent<CharacterComponent>(*(actor.get()));
            actor->Componentable::addComponent<LifeComponent>(*(actor.get()));
            
            actor->setState(STATE_FLYING);
 
            Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
            shader->use();
            shader->setUniform("size", 0.005f);
            shader->setUniform("brightness", 0.0f);
            
            Renderer::bindShaderUniblock(shader, ViewProj);
            Renderer::bindShaderUniblock(shader, Lights);

            Material map;
            AssetManager::loadTexture(TEX_FISH, &map.diffuse, true);
            AssetManager::loadTexture("Resources/Map/Screen Shot 2021-07-20 at 9.15.42 AM.png", &map.normMap, false);
             
            ActComp gc = std::make_shared<GraphicsComponent>(*(actor.get()), shader, map, DRAW_OPAQUE);
            static_pointer_cast<GraphicsComponent>(gc)->initModel(MOD_HOODY);
            actor->addComp(gc);

            actor->Componentable::addComponent<CombatComponent>(*(actor.get()));
            actor->Componentable::addComponent<PhysicsComponent>(*(actor.get()), false);
            ActComp ac = std::make_shared<AIComponent>(*(actor.get()));
            actor->addComp(ac);
            break;
        }
        
        case ACTOR_BIRD: {
            actor->Componentable::addComponent<NameComponent>(*(actor.get()));
            actor->Componentable::addComponent<LifeComponent>(*(actor.get()));
            actor->setState(STATE_FLYING);
     
            Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
            shader->use();
            shader->setUniform("size", 0.002);
            shader->setUniform("brightness", 0.5);
            Renderer::bindShaderUniblock(shader, ViewProj);
            Renderer::bindShaderUniblock(shader, DistanceFog);
            Renderer::bindShaderUniblock(shader, Lights);
             
            Material map;
            AssetManager::loadTexture("Resources/Models/bird/Tex_Ride_FengHuang_01a_D_A.tga.png", &map.diffuse, true);
            AssetManager::loadTexture("Resources/Map/Screen Shot 2021-07-20 at 9.15.42 AM.png", &map.normMap, false);
      
            ActComp gc = std::make_shared<GraphicsComponent>(*(actor.get()), shader, map, DRAW_OPAQUE);
            static_pointer_cast<GraphicsComponent>(gc)->initModel(MOD_BIRD);
            actor->addComp(gc);
            
            actor->Componentable::addComponent<CombatComponent>(*(actor.get()));
            actor->Componentable::addComponent<PhysicsComponent>(*(actor.get()), true);
            ActComp ac = std::make_shared<AnimComponent>(*(actor.get()), MOD_BIRD);
            static_pointer_cast<AnimComponent>(ac)->setDefaultAnim("Take 001");
            actor->addComp(ac);
            break;
 
        }
        case ACTOR_VAMP : {
            actor->Componentable::addComponent<CharacterComponent>(*(actor.get())); 
            actor->Componentable::addComponent<LifeComponent>(*(actor.get()));
            
            actor->setState(STATE_FLYING);
            ActComp ac = std::make_shared<AnimComponent>(*(actor.get()), MOD_VAMP);

            static_pointer_cast<AnimComponent>(ac)->setDefaultAnim("");
            actor->addComp(ac);

            Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
            shader->use();
            shader->setUniform("size", 0.005);
            shader->setUniform("brightness", 0.0);
            
            Renderer::bindShaderUniblock(shader, ViewProj);
            Renderer::bindShaderUniblock(shader, Lights);
            
            Material map;
            AssetManager::loadTexture("Resources/Models/Vampire/Vampire_diffuse.png", &map.diffuse, true);
            AssetManager::loadTexture("Resources/Map/Screen Shot 2021-07-20 at 9.15.42 AM.png", &map.normMap, false);
      
            actor->Componentable::addComponent<CombatComponent>(*(actor.get()));
            actor->Componentable::addComponent<PhysicsComponent>(*(actor.get()), true);
            

            ActComp gc = std::make_shared<GraphicsComponent>(*(actor.get()), shader, map, DRAW_OPAQUE);
            static_pointer_cast<GraphicsComponent>(gc)->initModel(MOD_VAMP);
            actor->addComp(gc);
            break;

        } 
    
    case ACTOR_SCARF_CHAR : {
        actor->Componentable::addComponent<NameComponent>(*(actor.get()));
        actor->Componentable::addComponent<InventoryComponent>(*(actor.get()), ItemNames);
        actor->Componentable::addComponent<LifeComponent>(*(actor.get()));
        actor->Componentable::addComponent<CollisionComponent>(*(actor.get()),*(actor.get()), AxisBounds(0.2f,-0.2f),AxisBounds(0.4f,-0.4f),AxisBounds(0.2f,-0.2f));
        
        Material map;
        AssetManager::loadTexture("Resources/Models/ScarfChar/scarfchar_diffuse.png", &map.diffuse, true);
          
        AssetManager::loadTexture(TEX_BLANK_NORMALS, &map.normMap, false);
 
        Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
        shader->use();
        shader->setUniform("animated", false); 
        shader->setUniform("size", 1.0);
        shader->setUniform("brightness", 0.0); 
          
        Renderer::bindShaderUniblock(shader,    ViewProj);
        Renderer::bindShaderUniblock(shader,      Lights);
        Renderer::bindShaderUniblock(shader,      DistanceFog);
          
        actor->Componentable::addComponent<PhysicsComponent>(*(actor.get()), true);
        
        ActComp gc = std::make_shared<GraphicsComponent>(*(actor.get()), shader, map, DRAW_OPAQUE);
        static_pointer_cast<GraphicsComponent>(gc)->initModel( "Resources/Models/ScarfChar/scarfchar.fbx");
        actor->addComp(gc);
        ActComp ac = std::make_shared<AnimComponent>(*(actor.get()),  "Resources/Models/ScarfChar/scarfchar.fbx");
 
        static_pointer_cast<AnimComponent>(ac)->setDefaultAnim("Idle");
        actor->addComp(ac);
        actor->setScale(0.005);
        break;  
            
    } 
        case ACTOR_KNIGHT : {
            actor->Componentable::addComponent<NameComponent>(*(actor.get()));
            actor->Componentable::addComponent<LifeComponent>(*(actor.get()));
        
            Material map;  
            AssetManager::loadTexture("Resources/Models/Knight/hollowknight.png", &map.diffuse, true);
            
            AssetManager::loadTexture(TEX_BLANK_NORMALS, &map.normMap, false);
 
            Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
            shader->use();
            shader->setUniform("size", 1.00);
            actor->setScale(0.003);
            shader->setUniform("brightness", 0.0);
              
            Renderer::bindShaderUniblock(shader,      ViewProj);
            Renderer::bindShaderUniblock(shader,      Lights);
            Renderer::bindShaderUniblock(shader, DistanceFog); 
              
            actor->Componentable::addComponent<PhysicsComponent>(*(actor.get()), true);
            
            ActComp gc = std::make_shared<GraphicsComponent>(*(actor.get()), shader, map, DRAW_OPAQUE);
            static_pointer_cast<GraphicsComponent>(gc)->initModel( "Resources/Models/Knight/hollowknight.fbx");
            actor->addComp(gc);
            ActComp ac = std::make_shared<AnimComponent>(*(actor.get()),  "Resources/Models/Knight/hollowknight.fbx");
     
            static_pointer_cast<AnimComponent>(ac)->setDefaultAnim("Passive");
            actor->addComp(ac);
            break;
               
        }   
        case ACTOR_FISH: { // maybe actor would want to know about ability field variables??
            actor->addComponent<LightComponent>(*actor.get(), *actor.get(), PointLight(glm::vec3(0.2,0.2,0.2), glm::vec3(3.5,3.5,3.5), glm::vec3(2.0,2.0,2.0), 1.0, 0.2, 0.2, glm::vec3(0)));
            Material map;   
            stbi_set_flip_vertically_on_load(1); 
            AssetManager::loadTexture(TEX_FISH, &map.diffuse, true); 
 
            Shader* shader = new Shader("Shaders/SketchVShader.vs", "Shaders/SketchFShader.fs");
            shader->use();
            shader->setUniform("alpha", 1.0);
            
            Renderer::bindShaderUniblock(shader,      ViewProj);
            Renderer::bindShaderUniblock(shader,      Lights);
            Renderer::bindShaderUniblock(shader,      StopWatch);
             
            ActComp gc = std::make_shared<GraphicsComponent>(*(actor.get()), shader, map, DRAW_OPAQUE);
            std::shared_ptr<VertexMesh> mesh = std::make_shared<VertexMesh>();
            int gridX = 20;
            int gridY = 3;
            float length = 0.8;  
            static_pointer_cast<GraphicsComponent>(gc)->initGrid(gridX, gridY, length, mesh);
            actor->addComp(gc); 
                
            ActComp mc = std::make_shared<MeshComponent>((*(actor.get())));
            static_pointer_cast<MeshComponent>(mc)->setMesh(mesh);
           
            Actor* actr = actor.get(); 
            std::shared_ptr<float> lastYaw = std::make_shared<float>(0.0f);
            std::shared_ptr<bool> first = std::make_shared<bool>(true);
                
            std::function<void (VertexMesh*)> meshAction = [=] (VertexMesh* v) { // idiot code to avoid making custom class lmao
                if (*(first.get())) {
                    *lastYaw.get() = actr->getYaw();
                    *first.get() = false;  
                    return;
                }
                 
                float deltayaw = actr->getYaw() - *lastYaw.get();
                deltayaw *= -1; //reverse
                deltayaw = glm::radians(deltayaw);
                *lastYaw.get() = actr->getYaw();
                
                for (int i = 0; i < gridX; i++) { // loop over columns
                    for (int j = 0; j < gridY; j++) { // loop through a column
                        if (i==0) {
                            //first column will always move with the actor pos center
                        } else {
                        glm::vec3& lastVertex = v->vertices.at(i+j*gridX).Pos;

                            lastVertex.x = lastVertex.x * cos(deltayaw) - lastVertex.z*sin(deltayaw);
                            lastVertex.z = lastVertex.x * sin(deltayaw) + lastVertex.z*cos(deltayaw);
                            // change of basis
                            lastVertex += glm::vec3(0.08,0,0);

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

            break; 
        }
        default: {
               
        Material map;
        AssetManager::loadTexture("Resources/Map/8grass.png", &map.diffuse, true);
            
        Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
        shader->use();
        shader->setUniform("size", 0.1);
        shader->setUniform("brightness", 0.0);
            
            Renderer::bindShaderUniblock(shader,      ViewProj);

            Renderer::bindShaderUniblock(shader,      Lights);
            
            actor->Componentable::addComponent<PhysicsComponent>(*(actor.get()), true);
            
            ActComp gc = std::make_shared<GraphicsComponent>(*(actor.get()), shader, map, DRAW_OPAQUE);
            static_pointer_cast<GraphicsComponent>(gc)->initModel("Resources/Map/grass1.fbx");
            actor->addComp(gc);
            actor->rotate(glm::vec3(-1.5,0,0));
            break;
        }
}
    stbi_set_flip_vertically_on_load(1);
    return actor;
}
