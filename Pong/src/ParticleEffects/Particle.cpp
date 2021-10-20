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
#include "AssetManager.hpp"
#include "World.hpp"

ParticleEffect::ParticleEffect() {
}


void ParticleEffect::init(float size_, glm::vec3 posVec_, glm::vec3 dimensions, int numParticles_, float ptcPerSec_, float duration_, float friction_) {
    graphics = new GraphicsComponent(); 
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
    
    setGraphics(shader); 
     
    graphics->setShader(shader);
    /**VertexData* data = new VertexData();
    
    if (drawTarget == GL_TRIANGLES) {
        data->generateTemplate(QUAD_SIMPLE);
    }
    
    if (drawTarget == GL_POINTS) {

        std::vector<GLuint> particleIndices;
        
        AnyVertex* v1 = new SimpleVertex(glm::vec3(0,0,0), glm::vec2(0,0), 0);

        std::vector<AnyVertex*> newVertices;
        newVertices.push_back(v1);
         
        data->setVertices(newVertices, VERTEX_SIMPLEVERTEX);
         
        for (int i = 0; i < numParticles; i++) {
            std::vector<GLuint> newIndices = {
                0
            };
            particleIndices.insert(particleIndices.end(), newIndices.begin(), newIndices.end());
        }
        data->setIndices(particleIndices);
    }
     **/
  //  graphics.init(data, shader); 
     
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
    
    if (world != NULL) {
          world->informParticlesForce(this);
    }
    
    float dt = glfwGetTime();
        
    if (myWatch.getTime() > 0.2) {
        myWatch.resetTime();
        for (int i = 0; i < ptcPerSec/5.0; i++) { 
        refreshParticle();
        if(firstUnused == (particles.size()-1)) {
            firstUnused = 0;
        }
        else if(particles[firstUnused+1].duration<=0) firstUnused++;
    }
    }
    
    if (forces.size() == 0) {
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
    } 
    else {
        for (int i = 0; i < particles.size(); i++) { 
            if (particles[i].duration > 0) {

             
                for (int j = 0; j < forces.size(); j++) { 
                    forces.at(j)->affectVelAt(particles[i].posVec, particles[i].velVec);
                }
                particles[i].velVec *= (friction);
              //  particles[i].velVec *= (friction-0.002*pow(glm::length(particles[i].velVec),2));
            if (glm::length(particles[i].velVec) < 0.03) particles[i].velVec = glm::vec3 (0,0,0);

                particles[i].duration -= dt;
                particles[i].posVec.x += particles[i].velVec.x*dt;
                particles[i].posVec.y += particles[i].velVec.y*dt;
                particles[i].posVec.z += particles[i].velVec.z*dt;
            }
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

void ParticleEffect::setWorld(World *world_) {
    world  = world_;
}
