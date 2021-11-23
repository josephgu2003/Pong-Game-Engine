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

std::shared_ptr<Prop> PropFactory::makeProp(int pe) { 
    std::shared_ptr<Prop> prop = std::make_shared<Prop>();
    auto foliageDraw = [] (Renderer* r, GraphicsComponent* gc) {
        r->renderFoliage(gc);
    }; 
    switch (pe) {
        case PROP_TREE: {
            TextureMaps map;
          //  AssetManager::loadTexture("Resources/Map/8grass.png", &map.diffuse, true);
            AssetManager::loadTexture("Resources/Map/tree/source/treediffuse.png", &map.diffuse, true);
            
            Shader* shader = new Shader("Shaders/ActorVertexShader.vs",  "Shaders/ActorFragmentShader.fs");
            shader->use();
            shader->setFloat("size", 0.02); 
            shader->setFloat("brightness", 0.0); 
                  
            Renderer::bindShaderUniblock(shader,      ViewProj);
   
            Renderer::bindShaderUniblock(shader,      Lights);
            
            std::shared_ptr<GraphicsComponent> gc = std::make_shared<GraphicsComponent>(*(prop.get()), shader, map);
            static_pointer_cast<GraphicsComponent>(gc)->initModel("Resources/Map/tree/source/tree2.fbx");
            prop->addComp(gc);
            break;
        }
            
        case PROP_GRASS: {
            TextureMaps map;
          //  AssetManager::loadTexture("Resources/Map/8grass.png", &map.diffuse, true);
            AssetManager::loadTexture("Resources/Map/flowers/flower.png", &map.diffuse, true);
            
            Shader* shader = new Shader("Shaders/ActorVertexShader.vs",  "Shaders/ActorFragmentShader.fs");
            shader->use();
            shader->setFloat("size", 0.9);
            shader->setFloat("brightness", 0.9);
                  
            Renderer::bindShaderUniblock(shader,      ViewProj); 
 
            Renderer::bindShaderUniblock(shader,      Lights); 
            
            std::shared_ptr<GraphicsComponent> gc = std::make_shared<GraphicsComponent>(*(prop.get()), shader, map);
            static_pointer_cast<GraphicsComponent>(gc)->initModel("Resources/Map/flowers/flowers.fbx");
            static_pointer_cast<GraphicsComponent>(gc)->setDrawCall(foliageDraw);
            prop->addComp(gc);
            prop->rotate(glm::vec3(glm::radians(-90.0f),0,0));
            break;
        }
            
        default:
            break;
    }
    return prop;
}
