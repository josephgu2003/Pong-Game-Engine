//
//  WorldEditor.hpp
//  Pong
//
//  Created by Joseph Gu on 3/1/22.
//

#ifndef WorldEditor_hpp
#define WorldEditor_hpp

#include <stdio.h>
#include "Component.hpp"
#include "Observer.hpp"
#include "uiText.hpp"
#include "json.hpp"

class Actor;
class uiLayout;

class WorldEditor : public Component, public Observer {
    std::weak_ptr<uiText> terminal;
    std::string terminalInput;
    nlohmann::json json;
    bool jsonLoaded;
public:
    WorldEditor(Actor& a, uiLayout* ui);
    void tick() override;
    void notify(const Subject &subject, GameEvent ge) override;
    
};
#endif /* WorldEditor_hpp */
