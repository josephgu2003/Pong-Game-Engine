//
//  PSpinComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 10/23/21.
//

#include "PSpinComponent.hpp"

PSpinComponent::PSpinComponent(ParticleSystem& pe, glm::vec3 spin_) : ParticleComp(pe){
    spin = spin_;
    particles = pe.getParticles();
    numParticles = pe.getNumParticles();
    type = ANIM; 
}
void PSpinComponent::tick() {
    for (int i = 0; i < numParticles; i++){
        particles[i].pyrAngles += spin;
    }
}
