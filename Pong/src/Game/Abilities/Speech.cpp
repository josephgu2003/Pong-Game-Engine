//
//  Speech.cpp
//  Pong
//
//  Created by Joseph Gu on 6/7/21.
//

#include "Speech.hpp"
#include "Particle.hpp"

Speech::Speech(Renderer* renderer_){
    renderer = renderer_;
}

void Speech::call() {
    renderer->print(lines.at(counter));
    counter++;
    running = true;
    on = true;
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
