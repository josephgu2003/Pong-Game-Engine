//
//  ChoiceMenu.hpp
//  Pong
//
//  Created by Joseph Gu on 3/9/22.
//

#ifndef ChoiceMenu_hpp
#define ChoiceMenu_hpp

#include <stdio.h>
#include "uiFrame.hpp"

class ChoiceMenu : public uiFrame {
public:
    ChoiceMenu(glm::vec2 pos, glm::vec2 dimensions, const std::string& filePath);
};

#endif /* ChoiceMenu_hpp */
