//
//  Speech.cpp
//  Pong
//
//  Created by Joseph Gu on 6/7/21.
//

#include "Speech.hpp"
#include "Particle.hpp"


Speech::Speech(World* world_, Actor* actor_, float duration_, std::vector<std::string> strings) : Ability(world_, actor_, duration_) {
    counter = 0;
    elapsetime = 0;
    running = false;
    lines = strings;
}

Speech::~Speech() {

}

void Speech::call(Game* game) {
    running = true;
    on = true;
    world->setActiveText(lines.at(counter));
    counter++;
}

void Speech::tick() {
    elapsetime+=glfwGetTime();
    if (counter == lines.size()) {
        running = false;
        on = false;
    }
    if (elapsetime > 4.3 && running) {
        world->setActiveText(lines.at(counter));
        counter++;
        elapsetime = 0;
    }
}
