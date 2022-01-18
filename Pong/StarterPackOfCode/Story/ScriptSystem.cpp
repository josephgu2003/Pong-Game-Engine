//
//  ScriptSystem.cpp
//  Pong
//
//  Created by Joseph Gu on 12/25/21.
//

#include "ScriptSystem.hpp"

bool ScriptSystem::isScriptComplete(const std::string& name) {
    for (auto i = allScripts.begin(); i != allScripts.end(); i++) {
        if ((*i)->getName() == name && (*i)->isComplete()) {
            return true;
        }
    } 
    return false;
}
 
void ScriptSystem::insertScript(std::shared_ptr<Script>& script) {
    allScripts.push_back(script);
} 

void ScriptSystem::tick() {
    for (auto i = allScripts.begin(); i != allScripts.end(); i++) {
        (*i)->tick(); 
    }
}

