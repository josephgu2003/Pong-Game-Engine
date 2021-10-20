//
//  uiText.hpp
//  Pong
//
//  Created by Joseph Gu on 10/16/21.
//

#ifndef uiText_hpp
#define uiText_hpp

#include <stdio.h>
#include "uiPiece.hpp"

class uiText : public uiPiece {
public:
    uiText();
    void setText(const std::string& s);
};
#endif /* uiText_hpp */