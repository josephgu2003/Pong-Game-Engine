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
#include "InventoryComponent.hpp"

Inventory::Inventory(std::weak_ptr<InventoryComponent>& invref, glm::vec2 pos, glm::vec2 scaling) : uiPiece(pos, scaling, "Shaders/UI.vs", "Shaders/GenericUI.fs"){
    nextPos = glm::vec2(-0.5,0.5);
    originalPos = nextPos;
    maxRowWidth = 1.4f; 
    pieceSize = glm::vec2(0.3,0.3);
    VertexLoader::load2DQuadData(VAO, VBO, EBO, numIndices, scaling, glm::vec2(0,0));
    AssetManager::loadTexture("Resources/GlyphsAndUI/inventory.png", &getSingularMaterial().diffuse, true);
    shader->use(); 
    shader->setUniform("alpha", 2.0f);
    setHiddenStatus(true);
    
    if (auto ic = invref.lock()) {
        const std::vector<GameItem>& items = ic->getItems();
        for (auto item : items) {
            int code = item.itemCode;
            loadItem(code);
        }
    }

} 
 
void Inventory::notify(const Subject& s, GameEvent ge) {
   if (ge == KEY_PRESSED) {
       if (static_cast<const InputHandler&>(s).getCurrentKeyPress() == GLFW_KEY_I) {
           if (isHidden())
           {
               initFadeFunction(0, 1000, 0.5f);
               setHiddenStatus(false);
           }
           else initFadeFunction(-0.5f, 0.5f, 0.5f); 
       }
   }
    if (ge == ACQUIRE_NEW_ITEM) {
        auto ic = static_cast<const InventoryComponent&>(s);
        auto itemlist = ic.getItems();
        for (auto item : itemlist) {
            if (loadedItems.find(item.itemCode) == loadedItems.end()) {
                loadItem(item.itemCode);
            }
        }
    }
}
