//
//  uiPiece.cpp
//  Pong
//
//  Created by Joseph Gu on 6/21/21.
//

#include "uiPiece.hpp"
#include "Renderer.hpp"

uiPiece::uiPiece() : Renderable::Renderable() {

} 

void uiPiece::draw(Renderer* r) {
    r->renderUI(this); 
}
