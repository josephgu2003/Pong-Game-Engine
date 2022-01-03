//
//  Inventory.cpp
//  Pong
//
//  Created by Joseph Gu on 12/29/21.
//

#include "Inventory.hpp"
#include "InputHandler.hpp"
#include "Renderer.hpp"
#include "VertexLoader.hpp" 

Inventory::Inventory(glm::vec2 pos, glm::vec2 scaling) : uiPiece(pos, scaling, "Shaders/UI.vs", "Shaders/GenericUI.fs"){
    VertexLoader::load2DQuadData(VAO, VBO, EBO, numIndices, scaling, glm::vec2(0,0));
    AssetManager::loadTexture("Resources/GlyphsAndUI/inventory.png", &getSingularMaterial().diffuse, true);
    shader->use(); 
    shader->setUniform("alpha", 2.0f);
    visible = false;
} 

void Inventory::notify(const Subject& s, GameEvent ge) {
   if (ge == KEY_PRESSED) {
       if (static_cast<const InputHandler&>(s).getCurrentKeyPress() == GLFW_KEY_I) {
           visible = !visible;
           watch.resetTime();
       }
   }
}
void Inventory::draw(Renderer* r) {
    if (visible) {
        float alpha = std::min(1.0f,watch.getTime());
        shader->use();
        shader->setUniform("alpha", alpha);
        r->renderGeneric(this);
    }
    if (!visible && watch.getTime() < 1.0f) {
        float alpha = std::min(1.0f,1.0f-watch.getTime());
        shader->use();
        shader->setUniform("alpha", alpha);
        r->renderGeneric(this);
    }
}
 
