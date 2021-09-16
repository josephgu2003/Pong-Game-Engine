//
//  uiLayout.hpp
//  Pong
//
//  Created by Joseph Gu on 6/21/21.
//

#ifndef uiLayout_hpp
#define uiLayout_hpp

#include <stdio.h>
#include <vector>
#include "uiPiece.hpp"

struct uiNode {
    uiPiece* piece = NULL;
    float x, y;
};

class uiLayout {
    std::vector<uiNode> uiPieces;
public:
    uiLayout();
    void insertNode(uiPiece* piece, float x, float y);
};
#endif /* uiLayout_hpp */
