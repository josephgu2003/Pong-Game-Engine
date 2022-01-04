//
//  InventoryItem.cpp
//  Pong
//
//  Created by Joseph Gu on 1/2/22.
//
 
#include "InventoryItem.hpp"
#include "VertexLoader.hpp"

InventoryItem::InventoryItem(const std::string& icon, glm::vec2 pos, glm::vec2 scaling) : uiPiece(pos, scaling, "Shaders/UI.vs", "Shaders/GenericUI.fs") {
    VertexLoader::load2DQuadData(VAO, VBO, EBO, numIndices, scaling, glm::vec2(0,0));
    AssetManager::loadTexture(icon.c_str(), &getSingularMaterial().diffuse, true);
    shader->use();
    shader->setUniform("alpha", 1.0f);
}
