//
//  ScriptFactory.cpp
//  Pong
//
//  Created by Joseph Gu on 11/13/21.
//

#include "ScriptFactory.hpp"
#include "ScriptWakeUp.hpp"

std::shared_ptr<Script> ScriptFactory::makeScript(int scriptenum, World* world) {
    std::shared_ptr<Script> script;
    switch (scriptenum) {
        case 0: {
            script = std::make_shared<ScriptWakeUp>(world);
            break;
        }
        default: { // !! make a dummy script
            script = std::make_shared<ScriptWakeUp>(world);
            break;
        }
    }
    return script; 
}
