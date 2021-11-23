//
//  uiPiece.cpp
//  Pong
//
//  Created by Joseph Gu on 6/21/21.
//

#include "uiPiece.hpp"
#include "Renderer.hpp"

uiPiece::uiPiece(glm::vec2 position_, glm::vec2 dimensions_) : GraphicsObject::GraphicsObject() {
    position = position_;
    dimensions = dimensions_;
} 

void uiPiece::draw(Renderer* r) {
    r->renderUI(this); 
}
 
void uiPiece::insertChild(const std::shared_ptr<uiPiece>& uip) {
    std::shared_ptr<uiPiece> ui = uip;
    children.push_back(std::move(ui));
}
 
