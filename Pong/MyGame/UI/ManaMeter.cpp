//
//  ManaMeter.cpp
//  Pong
//
//  Created by Joseph Gu on 12/3/21.
//

#include "ManaMeter.hpp"
#include "LifeComponent.hpp"
#include "VertexLoader.hpp"

ManaMeter::ManaMeter(glm::vec2 pos, glm::vec2 scaling) : uiPiece(pos, scaling, "Shaders/UI.vs", "Shaders/HealthMeter.fs") {
    VertexLoader::load2DQuadData(VAO, VBO, EBO, numIndices, scaling, glm::vec2(0,0));
    AssetManager::loadTexture("Resources/GlyphsAndUI/blueflower.png", &getSingularMaterial().diffuse, true);
    shader->use();
    shader->setUniform("ratio", 1.0f);  
}
 
void ManaMeter::notify(const Subject& s, GameEvent ge) {
    if (ge == SUBJ_MANA_CHANGED) {
       auto energy = static_cast<const LifeComponent&>(s).getStat(STAT_ENERGY);
        float ratio = energy.value / energy.max;
        shader->use();
        shader->setUniform("ratio", ratio);
    } 
}
 
