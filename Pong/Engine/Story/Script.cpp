//
//  ScriptEvent.cpp
//  Pong
//
//  Created by Joseph Gu on 7/2/21.
//

#include "Script.hpp"
#include "World.hpp"

Script::Script(World* world_, std::vector<std::string> crew) : Positionable() {
    world = world_;
    dummy = std::make_shared<Actor>(); 
    for (int i = 0; i < crew.size(); i++) { 
        actors.insert(std::pair<std::string, std::shared_ptr<Actor>>(crew.at(i), std::shared_ptr<Actor>(dummy)));
    }
    step = 0;
    stopWatch.resetTime();
    lastTime = -1.0f;
}

void Script::tick() {
    if (checkAllHere()) {
        act();
    }
}
 
bool Script::checkAllHere() { // slow???
    bool allHere = true;
    for (auto i = actors.begin() ; i != actors.end(); i++){
        auto x = world->getActorNamed(i->first);
        if ((x->dummy) || (x->getDistanceTo(this)) > 30.0) { 
            allHere = false;
        } else if (i->second->dummy){
            i->second = x;
        }
    }
    return allHere;
}

void Script::incStep(bool resetTime) {
    step++;
    if (resetTime) stopWatch.resetTime();
}
  
void Script::waitFor(float duration) {
    if (lastTime < 0.0f) {
        lastTime = stopWatch.getTime();
    }
    if ((stopWatch.getTime() - lastTime) > duration) {
        step++;
        lastTime = -1.0f;
    } 
}

bool Script::isWaiting() {
    if (lastTime < 0.0f) {
        return false;
    }
    return true;
}
