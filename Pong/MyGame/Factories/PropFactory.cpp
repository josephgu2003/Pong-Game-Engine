//
//  PropFactory.cpp
//  Pong
//
//  Created by Joseph Gu on 11/11/21.
//

#include "PropFactory.hpp"
#include <stdio.h>
#include <string>
#include "AssetManager.hpp"
#include "GraphicsComponent.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "VertexLoader.hpp" 
#include "CollisionComponent.hpp"
#include "PhysicsComponent.hpp"

std::shared_ptr<Prop> PropFactory::makeProp(int pe) {
    stbi_set_flip_vertically_on_load(0);
    std::shared_ptr<Prop> prop = std::make_shared<Prop>();
    auto foliageDraw = [] (Renderer* r, GraphicsComponent* gc) {
        r->renderFoliage(gc);
    };
    auto additiveDraw = [] (Renderer* r, GraphicsComponent* gc) {
        r->renderAdditiveBlend(gc);
    };
    switch (pe) {
        case PROP_WELL: {
            Material map;
            AssetManager::loadTexture("Resources/Models/Well/Well_Albedo.png", &map.diffuse, true);
            AssetManager::loadTexture("Resources/Models/Well/Well_NM.jpg", &map.normMap, false);
            
            Shader* shader = new Shader("Shaders/ActorVertexShader.vs",  "Shaders/ActorFragmentShader.fs");
            shader->use();
            shader->setUniform("size", 0.03f);
            shader->setUniform("brightness", 0.0f);
            
            std::shared_ptr<GraphicsComponent> gc =  std::make_shared<GraphicsComponent>(*(prop.get()), shader, map, DRAW_OPAQUE);
            static_pointer_cast<GraphicsComponent>(gc)->initModel(MOD_WELL);
            prop->addComp(gc);
            break;
        }
            
        case PROP_TREE: {
            Material map;
            AssetManager::loadTexture("Resources/Map/tree/source/treediffuse.png", &map.diffuse, true);
            
            Shader* shader = new Shader("Shaders/ActorVertexShader.vs",  "Shaders/ActorFragmentShader.fs");
            shader->use();
            shader->setUniform("size", 1.0f);
            shader->setUniform("brightness", 0.0f);
            
            std::shared_ptr<GraphicsComponent> gc = std::make_shared<GraphicsComponent>(*(prop.get()), shader, map, DRAW_OPAQUE);
            static_pointer_cast<GraphicsComponent>(gc)->initModel("Resources/Map/tree/source/tree2.fbx"); 
         //   gc->setDrawCall(foliageDraw); 
            prop->addComp(gc);
            break;
        }
              
        case PROP_MOUNT: {
            Material map;
            //  AssetManager::loadTexture("Resources/Map/8grass.png", &map.diffuse, true);
            AssetManager::loadTexture("Resources/Map/tree/source/treediffuse.png", &map.diffuse, true);
            
            Shader* shader = new Shader("Shaders/ActorVertexShader.vs",  "Shaders/ActorFragmentShader.fs");
            shader->use();
            shader->setUniform("size", 1.0f);
            shader->setUniform("brightness", 0.0f);
            
            std::shared_ptr<GraphicsComponent> gc = std::make_shared<GraphicsComponent>(*(prop.get()), shader, map, DRAW_OPAQUE);
            gc->initModel("Resources/Map/tree/source/tree2.fbx");
            prop->addComp(gc);
            break;
        }
            
        case PROP_GRASS: {
            Material map;
            AssetManager::loadTexture("Resources/Models/grass-patches/textures/grassAlphaMapped.png", &map.diffuse, true);
            
            Shader* shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
            shader->use();
            shader->setUniform("size", 0.01);
            shader->setUniform("brightness", 0.0);
            
            auto gc = std::make_shared<GraphicsComponent>(*(prop.get()), shader, map, DRAW_TRANSPARENT);
            gc->setDrawCall(foliageDraw);
            gc->initModel("Resources/Models/grass-patches/source/grasspatches.fbx");
            prop->addComp(gc);
            break;
        }
            
        case PROP_SWORD_SLASH: {
            Material map;
            
            AssetManager::loadTexture("Resources/Models/Sword/swordwave.png", &map.alphaMap, false);
            
            Shader* shader = new Shader("Shaders/SketchVShader.vs",  "Shaders/AlphaMap.fs");
            shader->use();
            shader->setUniform("color", glm::vec3(0.5, 0.5, 1.0));
            shader->setUniform("brightness", 5.0f);
            
            std::shared_ptr<GraphicsComponent> gc = std::make_shared<GraphicsComponent>(*(prop.get()), shader, map, DRAW_TRANSPARENT);
            gc->init([] (unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices) {
                std::vector<PosVertex> mesh;
                VertexLoader::loadSimpleVertexGrid(5, 3, 4.0, mesh, vao, vbo, ebo, numIndices);
            });
            prop->addComp(gc);
            prop->bakeRotation(glm::vec3(0, 80, 15));
            prop->addComponent<CollisionComponent>(*(prop.get()), AxisBounds(0.2f, -0.2f), AxisBounds(1.0f, 0.0f), AxisBounds(0.2f, -0.2f));
            break;
        }
            
        case PROP_WATER: {
            Material map;
            AssetManager::loadTexture(TEX_MIST, &map.diffuse, true);
            AssetManager::loadTexture("Resources/Map/snow.png", &map.normMap, false);
            Shader* shader = new Shader("Shaders/WaterVertexShader.vs",  "Shaders/WaterFragmentShader.fs");
            
            std::shared_ptr<GraphicsComponent> gc = std::make_shared<GraphicsComponent>(*(prop.get()), shader, map, DRAW_TRANSPARENT);
            gc->init([] (unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices) {
                std::vector<PosVertex> mesh;
                VertexLoader::loadSimpleVertexGrid(2, 2, 20.0, mesh, vao, vbo, ebo, numIndices);
            });
            prop->addComp(gc);
            prop->bakeRotation(glm::vec3(-90, 0, 0));
            break;
        }
            
        case PROP_LIGHTRAY: {
            Material map;  
            
            AssetManager::loadTexture("Resources/Particles/mist2.png", &map.alphaMap, false);
            
            Shader* shader = new Shader("Shaders/Fresnel.vs",  "Shaders/Moonbeam.fs");
            shader->use();
            shader->setUniform("color", glm::vec3(1.0,1.05,1.2));  
            shader->setUniform("brightness", 3.0f);
               
            std::shared_ptr<GraphicsComponent> gc =    std::make_shared<GraphicsComponent>(*(prop.get()), shader, map, DRAW_TRANSPARENT);
              
            gc->initModel("Resources/Abilities/moonbeam.fbx");
            gc->setDrawCall(additiveDraw);
            prop->addComp(gc);
            prop->bakeRotation(glm::vec3(90, 0 , 0));
            break; 
        }
            
        case PROP_IMPRISONMENT: {
            Material map;
            
            AssetManager::loadTexture("Resources/Particles/mist2.png", &map.alphaMap, false);
            
            Shader* shader = new Shader("Shaders/Fresnel.vs",  "Shaders/Imprisonment.fs");
            shader->use();
            shader->setUniform("color", glm::vec3(1.0,1.05,1.2));
            shader->setUniform("brightness", 3.0f); 
               
            std::shared_ptr<GraphicsComponent> gc =    std::make_shared<GraphicsComponent>(*(prop.get()), shader, map, DRAW_TRANSPARENT);
             
            gc->initModel("Resources/Abilities/imprisonment.fbx");
            gc->setDrawCall(additiveDraw);
            prop->addComp(gc);  
            prop->bakeRotation(glm::vec3(-90, 0 , 0)); 
            break;
        }
            
        case PROP_SWORD_EXP: {
            Material map;
            
            AssetManager::loadTexture("Resources/Particles/mist2.png", &map.alphaMap, false);
             
            Shader* shader = new Shader("Shaders/SketchVShader.vs",  "Shaders/SwordExplosion.fs");
            shader->use(); 
            shader->setUniform("color", glm::vec3(0.5,0.5,1.0));
            shader->setUniform("brightness", 5.0f);
            
            auto gc = std::make_shared<GraphicsComponent>(*(prop.get()), shader, map, DRAW_TRANSPARENT);
            
            gc->init([] (unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices) {
                VertexLoader::loadModelSimple("Resources/Models/Sword/swordexplosion.fbx", vao, vbo, ebo, numIndices);
            });
            gc->setDrawCall(additiveDraw);
            prop->addComp(gc);
            prop->bakeRotation(glm::vec3(90, 0 , 0));
            break;
        }
            
        case PROP_AURORA: {
            Material map; 
            
            AssetManager::loadTexture("Resources/Particles/mist2.png", &map.alphaMap, false);
            
            Shader* shader = new Shader("Shaders/Aurora.vs",  "Shaders/Aurora.fs");
            shader->use(); 
            shader->setUniform("color", glm::vec3(0.5,1.2,1.0)); 
            shader->setUniform("brightness", 3.0f);
              
            std::shared_ptr<GraphicsComponent> gc =   std::make_shared<GraphicsComponent>(*(prop.get()), shader, map, DRAW_TRANSPARENT);
            
            gc->init([] (unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices) {
                VertexLoader::loadModelSimple("Resources/Map/aurora.fbx", vao, vbo, ebo, numIndices); 
            });
            gc->setDrawCall(additiveDraw);
            prop->addComp(gc);
            prop->bakeRotation(glm::vec3(90, 0 , 0));
            break;
        }
            
    

        default:
            break;
    }
    stbi_set_flip_vertically_on_load(1);
    return prop;
}
