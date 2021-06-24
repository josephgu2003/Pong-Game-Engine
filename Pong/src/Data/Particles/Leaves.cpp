//
//  Leaves.cpp
//  Pong
//
//  Created by Joseph Gu on 6/10/21.
//

#include "Leaves.hpp"
#include "AssetManager.hpp"

void Leaves::init(float size_, glm::vec3 posVec_, float x_, float y_, float z_, int numParticles_, int cyclecount_) {
    size = size_;
    std::srand(314159);
    posVec = posVec_;
    texture = loadTexture(TEX_PEACH); //***
    x = x_;
    y = y_;
    z = z_;
    numParticles = numParticles_;
    for (int i = 0; i < numParticles; i++) {
        Particle particle;
        int j = (distribution(generator)%100);
        int k = (distribution(generator)%100);
        int l = (distribution(generator)%100);
        if (j%2 ==1) j = j*(-1);
        if (k%2 ==1) k = k*(-1);
        if (l%2 ==1) l = l*(-1);
        float m = x*j*0.01;
        float n = y*k*0.01;
        float o = z*l*0.01;
    particle.posVec = posVec+glm::vec3(m,n,o);
        particle.velVec = glm::vec3(0,0,0) + force;
        particle.texture = texture;
        particle.duration = 1;
    particles.push_back(particle);
    }
    cycle = cyclecount_;
    cyclecount = cyclecount_;
    firstUnused = 0;
    distribution = std::uniform_int_distribution<int>(1,1000);
}

void Leaves::tick() {
    float dt = glfwGetTime();
    for (int i = 0; i < particles.size(); i++) {
            particles[i].velVec += force;
        particles[i].velVec *= 0.95;
        if (glm::length(particles[i].velVec) < 0.03) particles[i].velVec = glm::vec3 (0,0,0);
            particles[i].posVec.x += particles[i].velVec.x*dt;
            particles[i].posVec.y += particles[i].velVec.y*dt;
            particles[i].posVec.z += particles[i].velVec.z*dt;
        }    
}
