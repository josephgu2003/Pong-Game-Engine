//
//  DevPosTracker.cpp
//  Pong
//
//  Created by Joseph Gu on 12/11/21.
//

#include "DevPosTracker.hpp"

#include "Positionable.hpp"

DevPosTracker::DevPosTracker(float xpos, float ypos, float xscale, float yscale) : uiText("", xpos, ypos, 0.0002f, DEFAULT_LINESPACE) {
     
}

void DevPosTracker::notify(const Subject& s, GameEvent ge) {
    auto setDisplayedText = [] (glm::vec3 pos) { 
        std::string newText = "";
        newText.append(std::to_string(float(pos.x)));
        newText.append(",\n ");
        newText.append(std::to_string(float(pos.y)));
        newText.append(",\n ");
        newText.append(std::to_string(float(pos.z))) ;
        return newText;
    }; 
    if (ge == SUBJ_POS_CHANGED) {
       auto subj = static_cast<const Positionable&>(s);
        glm::vec3 pos = subj.getPos();
        setText(setDisplayedText(pos));
    }
}
