//
//  Speech.cpp
//  Pong
//
//  Created by Joseph Gu on 6/7/21.
//

#include "Speech.hpp"
#include "Particle.hpp"
#include "NameComponent.hpp"
#include "Actor.hpp"

Speech::Speech(Actor* actor_, std::vector<std::string>& strings, std::vector<float>& durations_) {
    actor = actor_;
    step = 0;
    elapsetime = 0;
    lines = strings;
    durations = durations_;
    watch.resetTime();
}

void Speech::start() {
    if (!actor->hasComponent<NameComponent>()) {
        return; 
    }
    actor->getComponent<NameComponent>()->speak(lines.at(step), fetchDuration(step));
    step++;
}
  
void Speech::tick() {
    elapsetime += watch.getTime();
    watch.resetTime();
    if (step > lines.size()) {
        running = false;
        return; 
    }
    if (elapsetime > fetchDuration(step-1) && running) {
        if (step < lines.size())
            actor->getComponent<NameComponent>()->speak(lines.at(step), fetchDuration(step));
        step++;
        elapsetime = 0;
    }
} 
