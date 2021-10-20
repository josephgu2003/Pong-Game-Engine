//
//  uiLayout.cpp
//  Pong
//
//  Created by Joseph Gu on 6/21/21.
//

#include "uiLayout.hpp"
#include "Renderer.hpp"

uiLayout::uiLayout() {
}

void uiLayout::insertNode(const std::shared_ptr<uiPiece>& piece) {
    uiPieces.push_back(piece); //?
}

void uiLayout::renderAll(Renderer *renderer) {
    for (auto i : uiPieces) {
        renderer->renderUI(i.get());
    }
}
