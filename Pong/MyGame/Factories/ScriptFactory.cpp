//
//  ScriptFactory.cpp
//  Pong
//
//  Created by Joseph Gu on 11/13/21.
//

#include "ScriptFactory.hpp"
#include "ScriptWolfBoyOne.hpp"
#include "ScriptWakeUp.hpp"
#include "ScriptNightFlowers.hpp"

std::shared_ptr<Script> ScriptFactory::makeScript(std::string scriptname, World* world, bool completed) {
    std::shared_ptr<Script> script;
    if (scriptname == "FloroWakes") {
            script = std::make_shared<ScriptWakeUp>(world, completed, ui);
    } else if (scriptname == "Snowclaw"){
        script = std::make_shared<ScriptWolfBoyOne>(world, completed);
    } else if (scriptname == "Flowers in the Night") {
        script = std::make_shared<ScriptNightFlowers>(world, completed);
    } else { // !! make a dummy script 
            script = std::make_shared<ScriptWakeUp>(world, completed, ui);
    }
    return script; 
}

void ScriptFactory::setUI(const std::shared_ptr<uiLayout> ui_) {
    ui = ui_;
}
