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

void Speech::call(void* pointer) {
    running = true;
    on = true;
    renderer = static_cast<Renderer*>(pointer);
    renderer->print(lines.at(counter));
    counter++;
}

void Speech::tick() {
    elapsetime+=glfwGetTime();
    if (elapsetime > 4.3 && running) {
        renderer->print(lines.at(counter));
        counter++;
        elapsetime = 0;
    }
    if (counter == lines.size()) {
        running = false;
        on = false;
        this->~Speech();
    }
}
