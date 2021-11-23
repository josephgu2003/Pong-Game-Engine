//
//  uiMenu.cpp
//  Pong
//
//  Created by Joseph Gu on 10/24/21.
//

#include "uiMenu.hpp"
#include "Renderer.hpp"
#include "AssetManager.hpp"
#include "InputHandler.hpp"
#include "uiText.hpp"

uiMenu::uiMenu(glm::vec2 pos, glm::vec2 dimensions, const std::string& filePath) : uiPiece(pos, dimensions) {
    AssetManager::loadTexture(filePath.c_str(), &map.diffuse, true);
    setShader(new Shader("Shaders/Simple2D.vs", "Shaders/GenericUI.fs"));
    shader->use(); 
    shader->setFloat("ratio", 0.2);  
    VertexLoader::load2DQuadData(VAO, VBO, EBO, numIndices, dimensions, pos);
}

void uiMenu::draw(Renderer* r) {
    r->renderUI(this); 
    for (auto i : children) { 
        (i)->draw(r);
    }
}
 
void uiMenu::notify(const Subject& s, GameEvent ge) {
    auto x = static_cast<const InputHandler*>(&s);
    if (ge == KEY_PRESSED && x) {
        GLenum key = x->getCurrentKeyPress();
        handleKeyPress(key);
    }
}
 
void uiMenu::handleKeyPress(GLenum k) {
    switch (k) {
        case GLFW_KEY_DOWN: {
            std::static_pointer_cast<uiText>(children.at(selectedChild))->hover(false);
            int newIndex = selectedChild + 1;
            
            if (newIndex < children.size()) {
                selectedChild = newIndex;
            } else {
                selectedChild = 0;
            }
            // hover over selectedChild
            std::static_pointer_cast<uiText>(children.at(selectedChild))->hover(true);
            break;
        }
        case GLFW_KEY_ENTER: {
            std::static_pointer_cast<uiText>(children.at(selectedChild))->fire();
            break;
        }
             
        case GLFW_KEY_UP: {
            std::static_pointer_cast<uiText>(children.at(selectedChild))->hover(false);
            int newIndex = selectedChild-1;
            
            if (newIndex >= 0 ) {
                selectedChild = newIndex;
            } else {
                selectedChild = children.size()-1;
            }
            std::static_pointer_cast<uiText>(children.at(selectedChild))->hover(true);
            break;
        }
    }
}
