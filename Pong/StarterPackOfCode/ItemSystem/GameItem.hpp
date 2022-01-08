//
//  GameItem.hpp
//  Pong
//
//  Created by Joseph Gu on 1/2/22.
//

#ifndef GameItem_hpp
#define GameItem_hpp

#include <stdio.h>
#include "AssetManager.hpp"

struct GameItem {
    std::string name;
    unsigned int itemCode;
    GameItem(std::string name, unsigned int code);
};


#endif /* GameItem_hpp */
