//
//  ScriptEvent.cpp
//  Pong
//
//  Created by Joseph Gu on 7/2/21.
//

#include "Script.hpp"
#include "World.hpp"
#include "NameComponent.hpp"

Script::Script(World* world_, std::vector<std::string> crew, float radius_) : Positionable() {
    world = world_;
    radius = radius_;
    dummy = std::make_shared<Actor>();
    std::weak_ptr<Actor> ref = dummy;
    for (int i = 0; i < crew.size(); i++) { 
        actors.insert(std::pair<std::string, std::shared_ptr<Actor>>(crew.at(i), ref));
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
        if ((x->dummy) || (x->getDistanceTo(this)) > radius) {
            allHere = false;
        } else if (i->second.lock()->dummy){
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

void Script::newActor(std::string name, const std::shared_ptr<Actor>& actor) {
    actor->getComponent<NameComponent>()->init(name); 
    std::weak_ptr<Actor> ref = actor;
    actors.insert(std::pair<std::string, std::weak_ptr<Actor>>(name, ref));
    world->insert<Actor>(actor);
}

bool Script::isWaiting() {
    if (lastTime < 0.0f) {
        return false;
    }
    return true;
}

Actor* Script::getActorNamed(std::string name) {
    auto actor = actors.find(name);
    if (actor != actors.end()) {
        if (auto x = actor->second.lock()) {
            if (!x->dummy)
            return x.get();
        }
    } 
    return nullptr;
}
 
