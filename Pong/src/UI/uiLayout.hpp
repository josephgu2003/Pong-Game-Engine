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
#include <memory>

class Renderer;
 // lmao u dont make this a uipiece too lmao lmao
class uiLayout {
    std::vector<std::shared_ptr<uiPiece>> uiPieces;
public:
    uiLayout();
    void insertNode(const std::shared_ptr<uiPiece>& piece);
    void renderAll(Renderer* renderer);
   // void tick(); // really man? still ticking?
};

// stuff the uiPieces need to connect to:
// actors
// input system


// if insert a uiNode, how to remove it when necessary?
// some actor says some soundText, uiPiece in here
// when soundText is gone, uiPiece must also be gone
// uiPiece hold reference to SoundText?
#endif /* uiLayout_hpp */
