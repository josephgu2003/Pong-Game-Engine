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

/**ParticleEffect::ParticleEffect(glm::vec3 posVec_, float x_, float y_, float z_, int numParticles_, int cyclecount_) {
    loadGlyphs("Resources/Glyphs/minecraft-enchantment.ttf", glyphs);
    std::srand(314159);
    posVec = posVec_;
    x = x_;
    y = y_;
    z = z_;
    numParticles = numParticles_;
    for (int i = 0; i < numParticles; i++)
        particles.push_back(Particle());
    cycle = cyclecount_;
    cyclecount = cyclecount_;
    firstUnused = 0;
    distribution = std::uniform_int_distribution<int>(1,1000);
}**/

void ParticleEffect::init(float size_, glm::vec3 posVec_, float x_, float y_, float z_, int numParticles_, int cyclecount_) {
    size = size_;
    loadGlyphs("Resources/Glyphs/minecraft-enchantment.ttf", glyphs);
    std::srand(314159);
    posVec = posVec_;
    force = glm::vec3(0,0,0);
    x = x_;
    y = y_;
    z = z_;
    numParticles = numParticles_;
    for (int i = 0; i < numParticles; i++)
        particles.push_back(Particle());
    cycle = cyclecount_;
    cyclecount = cyclecount_;
    firstUnused = 0;
    texture = loadTexture(TEX_FIERY); //***
    distribution = std::uniform_int_distribution<int>(1,1000);
    shader = 3;
}

ParticleEffect::~ParticleEffect() {

}
void ParticleEffect::tick() {
    float dt = glfwGetTime();
    if (cycle == cyclecount) {
    cycle = 0;
    refreshParticle();
    if(firstUnused == (particles.size()-1)) {
        firstUnused = 0;
    }
    else if(particles[firstUnused+1].duration<=0) firstUnused++;
    }
    cycle++;
    for (int i = 0; i < particles.size(); i++) {
        if (particles[i].duration > 0) {
            particles[i].velVec += force;
            particles[i].velVec *= 0.98;
        if (glm::length(particles[i].velVec) < 0.03) particles[i].velVec = glm::vec3 (0,0,0);
            particles[i].posVec.x += particles[i].velVec.x*dt;
            particles[i].posVec.y += particles[i].velVec.y*dt;
            particles[i].posVec.z += particles[i].velVec.z*dt;
            particles[i].duration -= dt;
        }
    }
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
    float a = 0.01*(std::rand()%100);
    float b = 0.01*(std::rand()%100);
    float c = 0.01*(std::rand()%100);
  //  if (a%2 ==1) a = a*(-1);
   // b = b*(-1);
 //   c = c*(-1);
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

void ComplexParticles::init(float size_, glm::vec3 posVec_, float x_, float y_, float z_, int numParticles_, int cyclecount_) {
    ParticleEffect::init(size_, posVec_, x_, y_, z_, numParticles_,cyclecount_);
    shader = 4;
}

void ComplexParticles::refreshParticle() {
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
    float a = 0.01*(std::rand()%100);
    //int b = std::rand()%100;
    float c = 0.01*(std::rand()%100);
  //  if (a%2 ==1) a = a*(-1);
   // b = b*(-1);
 //   c = c*(-1);
particles[firstUnused].velVec = (glm::vec3(a-0.5,-0.2,c-0.5))+force;
particles[firstUnused].duration = 5;
particles[firstUnused].texture = glyphs[(char)((firstUnused%25)+97)].id;
}

