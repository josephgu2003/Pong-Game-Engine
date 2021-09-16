//
//  Speech.cpp
//  Pong
//
//  Created by Joseph Gu on 6/7/21.
//

#include "Speech.hpp"
#include "Particle.hpp"


Speech::Speech(World* world_, Actor* actor_, float duration_, std::vector<std::string> strings) : Ability(world_, actor_, duration_) {
    step = 0;
    elapsetime = 0;
    duration = duration_;
    running = false;
    lines = strings;
}

Speech::~Speech() {

}

void Speech::call() {
    running = true;
    on = true;
    world->setActiveText(lines.at(step));
    step++;
}
  
void Speech::tick() {
    elapsetime+=glfwGetTime();
    if (step > lines.size()) {
        running = false;
        on = false;
        return;
    }
    if (elapsetime > duration && running) {
        if (step < lines.size())
        world->setActiveText(lines.at(step));
        step++;
        elapsetime = 0;
    }
}
