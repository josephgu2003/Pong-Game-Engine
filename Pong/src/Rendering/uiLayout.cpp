//
//  uiLayout.cpp
//  Pong
//
//  Created by Joseph Gu on 6/21/21.
//

#include "uiLayout.hpp"

uiLayout::uiLayout() {
}
   
 


void uiLayout::insertNode(uiPiece* piece, float x, float y) {
    uiNode node;
    node.piece = piece;
    node.x = x;
    node.y = y;
    uiPieces.push_back(node);
}
