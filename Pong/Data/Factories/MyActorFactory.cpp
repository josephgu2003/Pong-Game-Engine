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

typedef std::shared_ptr<ActorComp> ActComp;

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
            
            ActComp gc = std::make_shared<GraphicsComponent>(*(actor.get()));
            Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
            shader->use();
            shader->setFloat("size", 0.005);
            shader->setFloat("brightness", 0.0);
                 
            Renderer::bindShaderUniblock(shader, ViewProj);
            Renderer::bindShaderUniblock(shader, Lights);
            TextureMaps map;
            AssetManager::loadTexture("Resources/Models/textures/lambert1_baseColor.png", &map.diffuse, true);
            AssetManager::loadTexture("Resources/Models/tmpugfolmqr", &map.normMap, false);
            static_pointer_cast<GraphicsComponent>(gc)->init(shader, MOD_HOODY, map);
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

            TextureMaps map;
            AssetManager::loadTexture(TEX_INKPAPER, &map.diffuse, true);
            AssetManager::loadTexture("Resources/Map/Screen Shot 2021-07-20 at 9.15.42 AM.png", &map.normMap, false);
             
            ActComp gc = std::make_shared<GraphicsComponent>(*(actor.get()));
            static_pointer_cast<GraphicsComponent>(gc)->init(shader, MOD_HOODY, map);
            actor->addComp(gc);

            actor->Componentable::addComp<CombatComponent>(*(actor.get()));
            actor->Componentable::addComp<PhysicsComponent>(*(actor.get()), false);
            ActComp ac = std::make_shared<AIComponent>(*(actor.get()));
            actor->addComp(ac);
            break;
        }
        
        case ACTOR_BIRD: {
            actor->Componentable::addComp<CharacterComponent>(*(actor.get()));
            actor->Componentable::addComp<LifeComponent>(*(actor.get()));
            actor->setState(STATE_FLYING);
     
            Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
            shader->use();
            shader->setFloat("size", 0.002);
            shader->setFloat("brightness", 0.0);
            Renderer::bindShaderUniblock(shader, ViewProj);

            Renderer::bindShaderUniblock(shader, Lights);
             
            TextureMaps map;
            AssetManager::loadTexture("Resources/Models/bird/Tex_Ride_FengHuang_01a_D_A.tga.png", &map.diffuse, true);
            AssetManager::loadTexture("Resources/Map/Screen Shot 2021-07-20 at 9.15.42 AM.png", &map.normMap, false);
      
            ActComp gc = std::make_shared<GraphicsComponent>(*(actor.get()));
            static_pointer_cast<GraphicsComponent>(gc)->init(shader, MOD_BIRD, map);
            actor->addComp(gc);
            
            actor->Componentable::addComp<CombatComponent>(*(actor.get()));
            actor->Componentable::addComp<PhysicsComponent>(*(actor.get()), true);
            ActComp ac = std::make_shared<AnimComponent>(*(actor.get()), MOD_BIRD);
            static_pointer_cast<AnimComponent>(ac)->setDefaultAnim("Take 001");
            actor->addComp(ac);
            ActComp aic = std::make_shared<AIComponent>(*(actor.get()));
            actor->addComp(aic);
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
            
            TextureMaps map;
            AssetManager::loadTexture("Resources/Models/Vampire/Vampire_diffuse.png", &map.diffuse, true);
            AssetManager::loadTexture("Resources/Map/Screen Shot 2021-07-20 at 9.15.42 AM.png", &map.normMap, false);
      
            actor->Componentable::addComp<CombatComponent>(*(actor.get()));
            actor->Componentable::addComp<PhysicsComponent>(*(actor.get()), true);
            

            ActComp gc = std::make_shared<GraphicsComponent>(*(actor.get()));
            static_pointer_cast<GraphicsComponent>(gc)->init(shader, MOD_VAMP, map);
            actor->addComp(gc);
            break;

        }
        case ACTOR_KNIGHT : {
            ActComp aic = std::make_shared<AIComponent>(*(actor.get()));
            actor->addComp(aic);
            actor->Componentable::addComp<CharacterComponent>(*(actor.get()));
            actor->Componentable::addComp<LifeComponent>(*(actor.get()));
        
            TextureMaps map; 
            AssetManager::loadTexture("/Users/josephgu/Downloads/hollow_knight_-_vr_chat_-_free_download/textures/Knight_baseColor.png", &map.diffuse, true);
            
            AssetManager::loadTexture("Resources/Models/tmpugfolmqr", &map.normMap, false);

            Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
            shader->use();
            shader->setFloat("size", 0.007);
            shader->setFloat("brightness", 0.5);
            
            Renderer::bindShaderUniblock(shader,      ViewProj);
            Renderer::bindShaderUniblock(shader,      Lights);
              
            actor->Componentable::addComp<PhysicsComponent>(*(actor.get()), true);
            actor->Componentable::addComp<CombatComponent>(*(actor.get()));
            
            ActComp gc = std::make_shared<GraphicsComponent>(*(actor.get()));
            static_pointer_cast<GraphicsComponent>(gc)->init(shader, "Resources/Models/Knight/knight.fbx", map);
            actor->addComp(gc);
            ActComp ac = std::make_shared<AnimComponent>(*(actor.get()), "Resources/Models/Knight/knight.fbx");

            static_pointer_cast<AnimComponent>(ac)->setDefaultAnim("HollowKnight__Armature|Walk");
            actor->addComp(ac);
            break;
            
        }
        default: {
               
        TextureMaps map;
        AssetManager::loadTexture("/Users/josephgu/Downloads/hollow_knight_-_vr_chat_-_free_download/textures/Knight_baseColor.png", &map.diffuse, true);
            
              
        Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
        shader->use();
        shader->setFloat("size", 0.007);
        shader->setFloat("brightness", 3.0);
            
            Renderer::bindShaderUniblock(shader,      ViewProj);

            Renderer::bindShaderUniblock(shader,      Lights);
            
            actor->Componentable::addComp<PhysicsComponent>(*(actor.get()), true);
            actor->Componentable::addComp<CombatComponent>(*(actor.get()));
            
            ActComp gc = std::make_shared<GraphicsComponent>(*(actor.get()));
        //   static_pointer_cast<GraphicsComponent>(gc)->setModel(model);
            actor->addComp(gc); 
            break; 
        }
}
    return actor;
}
