//
//  Particle.cpp
//  Pong
//
//  Created by Joseph Gu on 5/17/21.
//

#include "Particle.hpp"
#include <stdlib.h>
#include <time.h>
#include <memory>

ParticleEffect::ParticleEffect() {
}


void ParticleEffect::init(float size_, glm::vec3 posVec_, glm::vec3 dimensions, int numParticles_, float ptcPerSec_, float duration_, float friction_) {
    Shader* shader = new Shader();
    friction = friction_;
    size = size_;
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
    
    setGraphics(*shader);
    
    VertexData* data = new VertexData();
    
    if (drawTarget == GL_TRIANGLES) {

        std::vector<GLuint> particleIndices;
        
        std::shared_ptr<SimpleVertex> v1 = std::make_shared<SimpleVertex>(glm::vec3(0,0,0), glm::vec2(0,0));
        std::shared_ptr<SimpleVertex> v2 = std::make_shared<SimpleVertex>(glm::vec3(1,0,0), glm::vec2(1,0));
        std::shared_ptr<SimpleVertex> v3 = std::make_shared<SimpleVertex>(glm::vec3(1,1,0), glm::vec2(1,1));
        std::shared_ptr<SimpleVertex> v4 = std::make_shared<SimpleVertex>(glm::vec3(0,1,0), glm::vec2(0,1));

        std::vector<std::shared_ptr<AnyVertex>> newVertices = {
            v1, v2, v3, v4
        };
        
        data->setVertices(newVertices, VERTEX_SIMPLEVERTEX);
        
        for (int i = 0; i < numParticles; i++) {
            std::vector<GLuint> newIndices = {
                0, 1, 2,
                2, 3, 0
            };
            particleIndices.insert(particleIndices.end(), newIndices.begin(), newIndices.end());
        }
        data->setIndices(particleIndices);
    }
    
    if (drawTarget == GL_POINTS) {

        std::vector<GLuint> particleIndices;
        
        std::shared_ptr<SimpleVertex> v1 = std::make_shared<SimpleVertex>(glm::vec3(0,0,0), glm::vec2(0,0));

        std::vector<std::shared_ptr<AnyVertex>> newVertices = {
            v1
        };
        
        data->setVertices(newVertices, VERTEX_SIMPLEVERTEX);
        
        for (int i = 0; i < numParticles; i++) {
            std::vector<GLuint> newIndices = {
                0
            };
            particleIndices.insert(particleIndices.end(), newIndices.begin(), newIndices.end());
        }
        data->setIndices(particleIndices);
    }
    
    graphics.init(data, shader);
    
    extern GLuint uboViewProj;
    glBindBuffer(GL_UNIFORM_BUFFER, uboViewProj);
    glUniformBlockBinding(shader->ID, glGetUniformBlockIndex(shader->ID, "ViewProj"), 0);
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
    timer += dt;
        
    if (timer > 0.2) {
        timer = 0;
        for (int i = 0; i < ptcPerSec/5; i++) {
        refreshParticle();
        if(firstUnused == (particles.size()-1)) {
            firstUnused = 0;
        }
        else if(particles[firstUnused+1].duration<=0) firstUnused++;
    }
    }
    
    for (int i = 0; i < particles.size(); i++) {
        if (particles[i].duration > 0) {
            particles[i].velVec += force;
            particles[i].velVec *= friction;
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
    return particles.size();
}

Particle& ParticleEffect::getNthParticle(int n) {
    return particles[n];
}

float ParticleEffect::getSize() {
    return size;
}

void ParticleEffect::setForce(glm::vec3 force_) {
    force = force_;
}

