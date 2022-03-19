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
#include "Moonlight.hpp"
#include "Imprisonment.hpp"
#include "InTheWilderness.hpp"

ScriptFactory::ScriptFactory() {
    ih = NULL; 
}
std::shared_ptr<Script> ScriptFactory::makeScript(std::string scriptname, World* world, bool completed) {
    std::shared_ptr<Script> script; 
    if (scriptname == "FloroWakes") {
            script = std::make_shared<ScriptWakeUp>(world, completed, ui);
    } else if (scriptname == "Snowclaw"){
        script = std::make_shared<ScriptWolfBoyOne>(world, completed);
    } else if (scriptname == "Flowers in the Night") {
        script = std::make_shared<ScriptNightFlowers>(world, completed);
    } else if (scriptname == "Moonlight") {
        script = std::make_shared<Moonlight>(world, completed); 
    } else if (scriptname == "Imprisonment") {
        script = std::make_shared<Imprisonment>(world, completed, ui.lock().get(), ih);
    } else if (scriptname == "In the Wilderness") {
        script = std::make_shared<InTheWilderness>(world, completed, ih,  ui.lock().get());
    } else { // !! make a dummy script
            script = std::make_shared<ScriptWakeUp>(world, completed, ui);
    }
    return script; 
}

void ScriptFactory::setUI(const std::shared_ptr<uiLayout> ui_) {
    ui = ui_;
}

void ScriptFactory::setInputHandler(InputHandler *ih_) {
    ih = ih_;
}
