//
//  uiFrame.cpp
//  Pong
//
//  Created by Joseph Gu on 11/23/21.
//

#include "uiFrame.hpp"
#include "Renderer.hpp"
#include "VertexLoader.hpp"

uiFrame::uiFrame(glm::vec2 pos, glm::vec2 dimensions, const std::string& filePath) : uiPiece(pos, dimensions, "Shaders/UI.vs", "Shaders/GenericUI.fs") {
    AssetManager::loadTexture(filePath.c_str(), &getSingularMaterial().diffuse, true);
    shader->use();
    shader->setUniform("ratio", 0.2f); 
    shader->setUniform("alpha", 1.0f);
    VertexLoader::load2DQuadData(VAO, VBO, EBO, numIndices, dimensions, glm::vec3(0));
}

void uiFrame::draw(Renderer* r) {
    r->renderGeneric(this);
    for (auto i : children) {
        (i)->draw(r);
    }
}
 
