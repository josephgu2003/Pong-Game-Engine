//
//  HealthMeter.cpp
//  Pong
//
//  Created by Joseph Gu on 9/16/21.
//

#include "HealthMeter.hpp"
#include "LifeComponent.hpp"
#include "Shader.hpp"
#include "VertexLoader.hpp"
#include "AssetManager.hpp"

HealthMeter::HealthMeter() : uiPiece(){
    shader  = new Shader("Shaders/Simple2D.vs", "Shaders/HealthMeter.fs");
    VertexLoader::load2DQuadData(VAO, VBO, EBO, numIndices, glm::vec2(0.2,0.2), glm::vec2(-0.8, 0.8));
    AssetManager::loadTexture(TEX_FISH, &map.diffuse, true); 
    shader->use();
    shader->setFloat("ratio", 1.0f);
}
  
void HealthMeter::notify(const Subject& s, GameEvent ge) { 
    if (ge == SUBJ_HP_CHANGED) {
       auto hp = static_cast<const LifeComponent&>(s).getStat(STAT_LIFE);
        float ratio = hp.value / hp.max;
        shader->use(); 
        shader->setFloat("ratio", ratio);
    } 
} 
