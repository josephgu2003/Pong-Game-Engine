//
//  ParticleFactory.cpp
//  Pong
//
//  Created by Joseph Gu on 11/4/21.
//

#include "ParticleFactory.hpp"
#include "Shader.hpp"
#include "PGraphicsComponent.hpp"
#include "PPointGraphicsComponent.hpp"
#include "PQuadGraphicsComponent.hpp"
#include "AssetManager.hpp"
#include "PRefreshComponent.hpp"
#include "PPhysicsComponent.hpp"
#include "PSpinComponent.hpp"

std::shared_ptr<ParticleSystem> ParticleFactory::makeParticles(ParticleEffectSeed PESeed, glm::vec3 posVec_) {
    std::shared_ptr<ParticleSystem> particle;
    Shader* shader = new Shader();
    TextureMaps map;
    switch (PESeed) {
        case PE_FIREWORKS: {
            int numParticles = 200;
            particle = std::make_shared<ParticleSystem>(numParticles, 100.0);
            shader->init("Shaders/PointPart.vs", "Shaders/ColorPartF.fs");
            std::shared_ptr<PGraphicsComponent> pgc = std::make_shared<PPointGraphicsComponent>(*particle.get(), numParticles, 0.1, shader, map);
            pgc->setColor(0.5, 0.5, 8.0);
            particle->addComp(pgc);
            particle->addComp<PRefreshComponent>(*particle.get(), 3.0, numParticles, 3.0, glm::vec3(1,1,1), glm::vec3(-0.03, -0.03, -0.03), glm::vec3(0.03, 0.03, 0.03));
            particle->addComp<PPhysicsComponent>(*particle.get(), 0.002, 0.999);
            break;
        };
                
        case PE_MIST: {
            int numParticles = 1000;
            particle = std::make_shared<ParticleSystem>(numParticles, 1000.0);
            AssetManager::loadTexture(TEX_MIST, &map.diffuse, false);
            shader->init("Shaders/PointPart.vs", "Shaders/MistFShader.fs");
            particle->addComp<PPointGraphicsComponent>(*particle.get(), numParticles, 2.5, shader, map);
            particle->addComp<PRefreshComponent>(*particle.get(), 30.0, 10, 5.0, glm::vec3(20,2,20), glm::vec3(0, 0, 0), glm::vec3(0.02, 0, 0));
            particle->addComp<PPhysicsComponent>(*particle.get(), 0, 1.0);
            break;
        };
                
        case PE_BODYSPARKS:  {
            int numParticles = 200;
            particle = std::make_shared<ParticleSystem>(numParticles, 100.0);
            shader->init("Shaders/PointPart.vs", "Shaders/ColorPartF.fs");
            std::shared_ptr<PGraphicsComponent> pgc = std::make_shared<PPointGraphicsComponent>(*particle.get(), numParticles, 0.01, shader, map);
            pgc->setColor(0.5, 0.5, 3.0);
            particle->addComp(pgc);
            particle->addComp<PRefreshComponent>(*particle.get(), 3.0, 100, 0.2, glm::vec3(1,1,1), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
            break;
        };
                 
        case PE_RUNICLETTERS: {
            int numParticles = 30;
            particle = std::make_shared<ParticleSystem>(numParticles, 100.0);
            shader->init("Shaders/3DParticle.vs", "Shaders/GenericDiffuse.fs");
            shader->use();
            shader->setFloat("brightness", 5.0);
            AssetManager::loadTexture("Resources/Particles/symbol0.png", &map.diffuse, true);
            std::shared_ptr<PGraphicsComponent> pgc =  std::make_shared<PQuadGraphicsComponent>(*particle.get(), numParticles, 0.1, shader, map);
            particle->addComp(pgc);
            particle->addComp<PRefreshComponent>(*particle.get(), 2, 8, 0.5, glm::vec3(7,3,7), glm::vec3(-0.03, -0.03, 0), glm::vec3(0.03, -0.05, 0));
            particle->addComp<PPhysicsComponent>(*particle.get(), 0, 0.999);
            particle->addComp<PSpinComponent>(*particle.get(), glm::vec3(0,1,0));
            break;
        };
        case PE_SNOW: {
            int numParticles = 2000;
            particle = std::make_shared<ParticleSystem>(numParticles, 1000.0);
            shader->init("Shaders/PointPart.vs", "Shaders/ColorPartF.fs");
            std::shared_ptr<PGraphicsComponent> pgc = std::make_shared<PPointGraphicsComponent>(*particle.get(), numParticles, 0.03, shader, map);
            pgc->setColor(1.0, 1.0, 1.0);
            particle->addComp(pgc);
            particle->addComp<PRefreshComponent>(*particle.get(), 4.0, 300, 0.2, glm::vec3(10,9,10), glm::vec3(0, -0.03, 0), glm::vec3(0.08, -0.07, 0));
            particle->addComp<PPhysicsComponent>(*particle.get(), 0, 1);
            break;
        }; 
        default: {
            int numParticles = 100; 
            particle = std::make_shared<ParticleSystem>(numParticles, 100.0);
            shader->init("Shaders/3DParticle.vs", "Shaders/GenericDiffuse.fs");
            shader->use();
            shader->setFloat("brightness", 2.0);
            AssetManager::loadTexture("Resources/Particles/Project.png", &map.diffuse, true);
            std::shared_ptr<PGraphicsComponent> pgc =  std::make_shared<PQuadGraphicsComponent>(*particle.get(), numParticles, 0.3, shader, map);
            particle->addComp(pgc);
            particle->addComp<PRefreshComponent>(*particle.get(), 5.0, 2, 0.1, glm::vec3(5,5,5), glm::vec3(0, -0.1, 0), glm::vec3(0, 0, 0));
            particle->addComp<PPhysicsComponent>(*particle.get(), 0, 0.9985);
            particle->addComp<PSpinComponent>(*particle.get(), glm::vec3(0,1,0));
            break;
        };
    }
    particle->setPos(posVec_); 
    return particle;
}
 
 
