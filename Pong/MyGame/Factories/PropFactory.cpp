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

std::shared_ptr<Prop> PropFactory::makeProp(int pe) {
    stbi_set_flip_vertically_on_load(0);
    std::shared_ptr<Prop> prop = std::make_shared<Prop>();
    auto foliageDraw = [] (Renderer* r, GraphicsComponent* gc) {
        r->renderFoliage(gc);
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
                     
            Renderer::bindShaderUniblock(shader,      ViewProj);
            Renderer::bindShaderUniblock(shader,      DistanceFog); 
            Renderer::bindShaderUniblock(shader,      Lights);
            
            std::shared_ptr<GraphicsComponent> gc = std::make_shared<GraphicsComponent>(*(prop.get()), shader, map, DRAW_OPAQUE);
            static_pointer_cast<GraphicsComponent>(gc)->initModel(MOD_WELL);
            prop->addComp(gc);
            break;
        } 
        case PROP_TREE: {
            Material map;  
          //  AssetManager::loadTexture("Resources/Map/8grass.png", &map.diffuse, true);
            AssetManager::loadTexture("Resources/Map/tree/source/treediffuse.png", &map.diffuse, true);
            
            Shader* shader = new Shader("Shaders/ActorVertexShader.vs",  "Shaders/ActorFragmentShader.fs");
            shader->use();
            shader->setUniform("size", 1.0f); 
            shader->setUniform("brightness", 0.0f);
                   
            Renderer::bindShaderUniblock(shader,      ViewProj);
            Renderer::bindShaderUniblock(shader,      DistanceFog); 
            Renderer::bindShaderUniblock(shader,      Lights);
            
            std::shared_ptr<GraphicsComponent> gc = std::make_shared<GraphicsComponent>(*(prop.get()), shader, map, DRAW_OPAQUE);
            static_pointer_cast<GraphicsComponent>(gc)->initModel("Resources/Map/tree/source/tree2.fbx");
            prop->addComp(gc);
            break; 
        }
            
        case PROP_GRASS: {
            Material map;
          //  AssetManager::loadTexture("Resources/Map/8grass.png", &map.diffuse, true);
            AssetManager::loadTexture("Resources/Map/flowers/flower.png", &map.diffuse, true);
            
            Shader* shader = new Shader("Shaders/ActorVertexShader.vs",  "Shaders/ActorFragmentShader.fs");
            shader->use();
            shader->setUniform("size", 0.9f);
            shader->setUniform("brightness", 0.9f);
                  
            Renderer::bindShaderUniblock(shader,      ViewProj); 
 
            Renderer::bindShaderUniblock(shader,      Lights); 
            
            std::shared_ptr<GraphicsComponent> gc = std::make_shared<GraphicsComponent>(*(prop.get()), shader, map, DRAW_TRANSPARENT);
            static_pointer_cast<GraphicsComponent>(gc)->initModel("Resources/Map/flowers/flowers.fbx");
            static_pointer_cast<GraphicsComponent>(gc)->setDrawCall(foliageDraw);
            prop->addComp(gc);
            prop->rotate(glm::vec3(glm::radians(-90.0f),0,0));
            break; 
        }
             
        case PROP_SWORD_SLASH: {
            Material map;
            
            AssetManager::loadTexture("Resources/Models/Sword/swordwave.png", &map.diffuse, true);
            
            Shader* shader = new Shader("Shaders/SketchVShader.vs",  "Shaders/AlphaMap.fs");
            shader->use();
            shader->setUniform("color", glm::vec3(0.0,0.2,1.0));
            shader->setUniform("brightness", 10.0f); 
            Renderer::bindShaderUniblock(shader, ViewProj);
            
            std::shared_ptr<GraphicsComponent> gc = std::make_shared<GraphicsComponent>(*(prop.get()), shader, map, DRAW_TRANSPARENT);
            gc->init([] (unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices) {
  
                std::vector<PosVertex> mesh;
                VertexLoader::loadSimpleVertexGrid(5, 3, 4.0, mesh, vao, vbo, ebo, numIndices);
            });
            prop->addComp(gc);
            prop->bakeRotation(glm::vec3(0,80,15));
            prop->addComponent<CollisionComponent>(*(prop.get()), *(prop.get()), AxisBounds(0.2f,-0.2f),AxisBounds(0.3f,-0.3f),AxisBounds(0.2f,-0.2f));
            break;
        }
            
        default:
            break;
    }
    stbi_set_flip_vertically_on_load(1);
    return prop;
}
