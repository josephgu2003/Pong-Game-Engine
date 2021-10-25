//
//  uiMenu.cpp
//  Pong
//
//  Created by Joseph Gu on 10/24/21.
//

#include "uiMenu.hpp"
#include "Renderer.hpp"
#include "AssetManager.hpp"

uiMenu::uiMenu(glm::vec2 pos, glm::vec2 dimensions, const std::string& filePath) : uiPiece(pos, dimensions) {
    AssetManager::loadTexture(filePath.c_str(), &map.diffuse, true);
    setShader(new Shader("Shaders/Simple2D.vs", "Shaders/GenericUI.fs"));
    shader->use(); 
    shader->setFloat("ratio", 0.2);  
    VertexLoader::load2DQuadData(VAO, VBO, EBO, numIndices, dimensions, pos);
}

void uiMenu::draw(Renderer* r) {
    r->renderElements(this); 
    for (auto i : children) {
        (i)->draw(r);
    }
} 
