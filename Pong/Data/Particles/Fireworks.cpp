//
//  Fireworks.cpp
//  Pong
//
//  Created by Joseph Gu on 7/3/21.
//

#include "Fireworks.hpp"


Fireworks::Fireworks() {
    
}
Fireworks::Fireworks(glm::vec4 color_) {
    color = color_;
}

void Fireworks::setGraphics(Shader& shader) {
    force = glm::vec3(0.0,-0.05,0.0);
    firstUnusedTrail = numParticles;
    particleRefresh = 0.03;
    numberSparks = numParticles;
    numParticles = numParticles * 50;
    for(int i = numberSparks; i < numParticles; i++) {
        particles.push_back(Particle());
    }
    AssetManager::loadTexture(TEX_GRADIENT, &texture, false);
    textureTarget = GL_TEXTURE_2D;
    drawTarget = GL_POINTS;
    shader.init("Shaders/ColorPartV.vs", "Shaders/ColorPartF.fs");
    shader.use();
    shader.setVec4("color", color);
    shader.setFloat("size", size);
}

void Fireworks::setColor(glm::vec4 color_) {
    color = color_;
    graphics.getShader()->use();
    graphics.getShader()->setVec4("color", color);
}
void Fireworks::tick() {
    if (actor != NULL) {
    posVec = actor->getPos();
    }
    
    float dt = glfwGetTime();
    timer += dt;
    timer2 += dt;
    
    if (!exploded) {
        for (int i = 0; i < numberSparks; i++) {
        refreshParticle();
        if(firstUnused == (numberSparks-1)) {
            firstUnused = 0;
        }
        else if(particles[firstUnused+1].duration<=0) firstUnused++;
    }
        exploded = true;
    }
    if (timer2 > 5.0) {
        exploded = false;
        timer2 = 0;
    }
    for (int i = 0; i < numberSparks; i++) {
        if (particles[i].duration > 0) {
            particles[i].velVec += force;
           particles[i].velVec *= friction;
        if (glm::length(particles[i].velVec) < 0.03) particles[i].velVec = glm::vec3 (0,0,0);
            if (timer > particleRefresh) {
                particles[firstUnusedTrail].posVec = particles[i].posVec;
                particles[firstUnusedTrail].velVec = glm::vec3(0.0);
   
                particles[firstUnusedTrail].duration = 0.5;
                 
                particles[firstUnusedTrail].texture = texture.id;
                if(firstUnusedTrail == (particles.size()-1)) {
                    firstUnusedTrail = numberSparks;
                }
                else if(particles[firstUnusedTrail+1].duration<=0) firstUnusedTrail++;
            }
            particles[i].posVec.x += particles[i].velVec.x*dt;
            particles[i].posVec.y += particles[i].velVec.y*dt;
            particles[i].posVec.z += particles[i].velVec.z*dt;
            particles[i].duration -= dt;
        }
    }
    for (int i = numberSparks; i < particles.size(); i++) {
        if (particles[i].duration > 0) {
            particles[i].duration -= dt;
        }
    }
    duration -= dt;
    if (timer > particleRefresh) {
      //  particleRefresh = 0.02 + 0.0002*(distribution(generator)%100);
        timer = 0;
    }
}

void Fireworks::refreshParticle() {
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
    float a = 0.003*(distribution(generator)%100);
    float b = 0.003*(distribution(generator)%100);
    float c = 0.003*(distribution(generator)%100);
    a -= 0.15;
    b -= 0.15;
    c -= 0.15;
particles[firstUnused].velVec = 4.0f*glm::normalize(glm::vec3(a,b,c));
particles[firstUnused].duration = 2.0f;
particles[firstUnused].texture = texture.id;
}
