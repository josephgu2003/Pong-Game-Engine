//
//  uiMenu.hpp
//  Pong
//
//  Created by Joseph Gu on 10/24/21.
//

#ifndef uiMenu_hpp
#define uiMenu_hpp

#include <stdio.h>
#include "uiPiece.hpp"

class uiMenu : public uiPiece {
public:
    uiMenu(glm::vec2 pos, glm::vec2 dimensions, const std::string& filePath);
    virtual void draw(Renderer* r) override;
};
#endif /* uiMenu_hpp */
