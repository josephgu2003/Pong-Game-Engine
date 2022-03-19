//
//  Imprisonment.hpp
//  Pong
//
//  Created by Joseph Gu on 3/4/22.
//

#ifndef Imprisonment_hpp
#define Imprisonment_hpp

#include <stdio.h>
#include "Script.hpp"
#include "Prop.hpp"
#include "uiText.hpp"
#include "uiMenu.hpp"

class uiLayout;
class InputHandler;

class Imprisonment : public Script {
private:
    std::weak_ptr<Prop> imprisonment;
    Watch watch;
    float wallCooldown;
    float imprisonmentBrightness;
    Watch brightnessWatch;
    uiLayout* ui = nullptr;
    InputHandler* ih = nullptr;
    std::vector<std::shared_ptr<uiText>> dreamSequence;
    std::weak_ptr<uiMenu> choiceMenu;
public:
    Imprisonment(World* world, bool completed, uiLayout* ui, InputHandler* ih);
    virtual void act() override;
};

#endif /* Imprisonment_hpp */
