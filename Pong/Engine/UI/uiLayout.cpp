//
//  uiLayout.cpp
//  Pong
//
//  Created by Joseph Gu on 6/21/21.
//

#include "uiLayout.hpp"
#include "Renderer.hpp"

uiLayout::uiLayout() {
    activePiece = std::weak_ptr<uiPiece>();
}

void uiLayout::insertNode(const std::shared_ptr<uiPiece>& piece) {
    for (auto i : uiPieces) {
        if (i.get() == piece.get()) {
            return;
        }
    }
    uiPieces.push_back(piece); //? 
}

void uiLayout::renderAll(Renderer *renderer) {
    for (auto i : uiPieces) {
       i->draw(renderer);
    }
}
 
void uiLayout::removeNode(uiPiece* piece) {
    for (int i = 0; i < uiPieces.size(); i++) {
        if (uiPieces.at(i).get() == piece) {
            // delete piece code
            uiPieces.erase(uiPieces.begin()+i);
            return;
        }
    }
}

void uiLayout::deleteActivePopup() {
    if (auto x = activePiece.lock()) {
        removeNode(x.get());
    } 
    activePiece.reset();
}

void uiLayout::setActivePopup(const std::shared_ptr<uiPiece>& piece) {
    activePiece = piece;
    insertNode(piece); 
}
 

void uiLayout::clear() {
    activePiece.reset();
    uiPieces.clear(); 
}

void uiLayout::removeDeadUI() {
    for (int i = 0; i < uiPieces.size(); i++) {
        if (uiPieces.at(i)->shouldDeleteThis()) {
            uiPieces.erase(uiPieces.begin()+i);
            return;
        }
    }
}
