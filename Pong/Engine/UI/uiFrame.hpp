//
//  uiFrame.hpp
//  Pong
//
//  Created by Joseph Gu on 11/23/21.
//

#ifndef uiFrame_hpp
#define uiFrame_hpp

#include <stdio.h>
#include "uiPiece.hpp"

class uiFrame : public uiPiece {
protected:
public:
    uiFrame(glm::vec2 pos, glm::vec2 dimensions, const std::string& filePath);
    virtual void draw(Renderer* r) override;
};
#endif /* uiFrame_hpp */
