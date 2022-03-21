//
//  PSpinComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 10/23/21.
//

#include "PSpinComponent.hpp"
#include "Particle.hpp"

PSpinComponent::PSpinComponent(ParticleSystem& pe, glm::vec3 spin_) : Component(pe){
    spin = spin_;
    particles = pe.getParticles();
    numParticles = pe.getNumParticles();
    updatePriority = 3;
}
void PSpinComponent::tick() {
    for (int i = 0; i < numParticles; i++){
        particles[i].pyrAngles += spin;
    }
}
