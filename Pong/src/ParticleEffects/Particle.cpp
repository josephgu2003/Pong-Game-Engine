//
//  Particle.cpp
//  Pong
//
//  Created by Joseph Gu on 5/17/21.
//

#include "Particle.hpp"
#include "World.hpp"
#include "PGraphicsComponent.hpp"
#include "PPointGraphicsComponent.hpp"
#include "PQuadGraphicsComponent.hpp"
#include "AssetManager.hpp"
#include "PRefreshComponent.hpp"
#include "PPhysicsComponent.hpp"
#include "PSpinComponent.hpp"

ParticleSystem::ParticleSystem() {
 
}

void ParticleSystem::init(ParticleEffectSeed PESeed, glm::vec3 posVec_) {
    posVec = posVec_;
    Shader* shader;
    TextureMaps map;
      
    switch (PESeed) {
        case PE_FIREWORKS: {
            numParticles = 200;
            particles = new Particle[numParticles];
            duration = 100.0f;
            shader = new Shader("Shaders/ColorPartV.vs", "Shaders/ColorPartF.fs");
            std::shared_ptr<PGraphicsComponent> pgc = std::make_shared<PPointGraphicsComponent>(*this, numParticles, 0.1, shader, map);
            pgc->setColor(0.5, 0.5, 8.0);
            addComp(pgc); 
            std::shared_ptr<PRefreshComponent> prc = std::make_shared<PRefreshComponent>(*this, 3.0, 200, 3.0, glm::vec3(1,1,1), glm::vec3(-0.03, -0.03, -0.03), glm::vec3(0.03, 0.03, 0.03));
            addComp(prc);
            std::shared_ptr<PPhysicsComponent> ppc = std::make_shared<PPhysicsComponent>(*this, 0.002, 0.999);
            addComp(ppc);
            break;
        }; 
            
        case PE_MIST: {
            numParticles = 500;
            particles = new Particle[numParticles];
            duration = 1000.0f;
            AssetManager::loadTexture(TEX_MIST, &map.diffuse, false);
            shader = new Shader("Shaders/MistVShader.vs", "Shaders/MistFShader.fs");
            std::shared_ptr<PGraphicsComponent> pgc = std::make_shared<PPointGraphicsComponent>(*this, numParticles, 2.5, shader, map);
            addComp(pgc);
            std::shared_ptr<PRefreshComponent> prc =   std::make_shared<PRefreshComponent>(*this, 10.0, 2, 20.0, glm::vec3(40,2,40), glm::vec3(0, 0, 0), glm::vec3(0.1, 0, 0));
            addComp(prc);
            std::shared_ptr<PPhysicsComponent> ppc = std::make_shared<PPhysicsComponent>(*this, 0, 1.0);
            addComp(ppc);
            break;
        };
            
        case PE_BODYSPARKS:  {
            numParticles = 200;
            particles = new Particle[numParticles];
            duration = 100.0f;
            shader = new Shader("Shaders/ColorPartV.vs", "Shaders/ColorPartF.fs");
            std::shared_ptr<PGraphicsComponent> pgc = std::make_shared<PPointGraphicsComponent>(*this, numParticles, 0.01, shader, map);
            pgc->setColor(0.5, 0.5, 3.0);
            addComp(pgc);
            std::shared_ptr<PRefreshComponent> prc = std::make_shared<PRefreshComponent>(*this, 3.0, 100, 0.2, glm::vec3(1,1,1), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
            addComp(prc);
            break;
        };
             
        case PE_RUNICLETTERS: {
            numParticles = 100;
            particles = new Particle[numParticles];
            duration = 100.0f;
            shader = new Shader("Shaders/3DParticle.vs", "Shaders/GenericDiffuse.fs");
            shader->use();
            shader->setFloat("brightness", 2.0);
            AssetManager::loadTexture("Resources/Particles/Project.png", &map.diffuse, true);
            std::shared_ptr<PGraphicsComponent> pgc =  std::make_shared<PQuadGraphicsComponent>(*this, numParticles, 0.3, shader, map);
            addComp(pgc); 
            std::shared_ptr<PRefreshComponent> prc = std::make_shared<PRefreshComponent>(*this, 5.0, 2, 0.1, glm::vec3(5,5,5), glm::vec3(0, -0.1, 0), glm::vec3(0, 0, 0));
            addComp(prc);  
            std::shared_ptr<PPhysicsComponent> ppc = std::make_shared<PPhysicsComponent>(*this, 0, 0.9985);
            addComp(ppc);
            std::shared_ptr<PSpinComponent> psc = std::make_shared<PSpinComponent>(*this, glm::vec3(0,1,0));
            addComp(psc);
            break;
        };
    }  
    
    for (int i = 0; i < numParticles; i++)
    {
        particles[i] = Particle();
    }
    
}


ParticleSystem::~ParticleSystem() {

}

void ParticleSystem::tick() {
    Componentable::tick();
    for (int i = 0; i < numParticles; i++) {
        particles[i].duration -= glfwGetTime();
    }
    duration -= glfwGetTime();
}

int ParticleSystem::getNumParticles() {
    return numParticles;
}

Particle& ParticleSystem::getNthParticle(int n) {
    return particles[n];
}

void ParticleSystem::setWorld(World *world_) {
    world  = world_;
}

Particle* ParticleSystem::getParticles() {
    return particles;
}
const glm::vec3& ParticleSystem::getPos() {
    return posVec;
}
 
