//
//  Particle.cpp
//  Pong
//
//  Created by Joseph Gu on 5/17/21.
//

#include "Particle.hpp"
#include <stdlib.h>
#include <time.h>

ParticleEffect::ParticleEffect() {
}


void ParticleEffect::init(float size_, glm::vec3 posVec_, glm::vec3 dimensions, int numParticles_, float ptcPerSec_, float duration_) {
    size = size_;
    std::srand(314159);
    posVec = posVec_;
    force = glm::vec3(0,0,0);
    x = dimensions.x;
    y = dimensions.y;
    z = dimensions.z;
    numParticles = numParticles_;
    ptcPerSec = ptcPerSec_;
    duration = duration_;
    
    for (int i = 0; i < numParticles; i++)
        particles.push_back(Particle());
    
    firstUnused = 0;

    distribution = std::uniform_int_distribution<int>(1,1000);
    

    
    extern GLuint uboViewProj;
    glBindBuffer(GL_UNIFORM_BUFFER, uboViewProj);
    GLuint viewproj  = glGetUniformBlockIndex(shader.ID, "ViewProj");
    glUniformBlockBinding(shader.ID, glGetUniformBlockIndex(shader.ID, "ViewProj"), 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboViewProj);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ParticleEffect::setActor(Actor* actor_) {
    actor = actor_;
}

ParticleEffect::~ParticleEffect() {

}

void ParticleEffect::tick() {
    if (actor != NULL) {
    posVec = actor->getPos();
    }
    
    float dt = glfwGetTime();
        
        for (int i = 0; i < 3; i++) {
        refreshParticle();
        if(firstUnused == (particles.size()-1)) {
            firstUnused = 0;
        }
        else if(particles[firstUnused+1].duration<=0) firstUnused++;
    }
    
    for (int i = 0; i < particles.size(); i++) {
        if (particles[i].duration > 0) {
            particles[i].velVec += force;
            particles[i].velVec *= 0.995;
        if (glm::length(particles[i].velVec) < 0.03) particles[i].velVec = glm::vec3 (0,0,0);
            particles[i].posVec.x += particles[i].velVec.x*dt;
            particles[i].posVec.y += particles[i].velVec.y*dt;
            particles[i].posVec.z += particles[i].velVec.z*dt;
            particles[i].duration -= dt;
        }
    }
    duration -= dt;
}

int ParticleEffect::getNumParticles() {
    return numParticles;
}

Particle& ParticleEffect::getNthParticle(int n) {
    return particles[n];
}

void ParticleEffect::refreshParticle() {
    int j = (distribution(generator)%100);
    int k = (distribution(generator)%100);
    int l = (distribution(generator)%100);
    if (j%2 ==1) j = j*(-1);
    if (k%2 ==1) k = k*(-1);
    if (l%2 ==1) l = l*(-1);
    float m = x*j*0.01;
    float n = y*k*0.01;
    float o = z*l*0.01;
    particles[firstUnused].posVec = posVec+glm::vec3(m,n,o);
    float a = 0.01*(distribution(generator)%100);
    float b = 0.01*(distribution(generator)%100);
    float c = 0.01*(distribution(generator)%100);
particles[firstUnused].velVec = (glm::vec3(1*a-0.5,1*b-0.25,1*c-0.5));
particles[firstUnused].duration = 1;
particles[firstUnused].texture = texture;
}

float ParticleEffect::getSize() {
    return size;
}

void ParticleEffect::setForce(glm::vec3 force_) {
    force = force_;
}
