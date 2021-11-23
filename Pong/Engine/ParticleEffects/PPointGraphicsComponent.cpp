//
//  PPointGraphicsComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 10/21/21.
//

#include "PPointGraphicsComponent.hpp"
#include "VertexLoader.hpp"
#include "Particle.hpp"
#include "PRefreshComponent.hpp"

PPointGraphicsComponent::PPointGraphicsComponent(ParticleSystem& pe, int numberParticles, float size, Shader* shader, const TextureMaps& map_) : PGraphicsComponent(pe, size, shader, map_){
    int datasize = numberParticles * 4 * sizeof(float);
    std::vector<int> v = {3,1};
    VertexLoader::loadPoint(VAO, VBO, EBO, numIndices);
    makeInstanceBuffer(datasize, 2, v, static_cast<ParticleSystem*>(actor)->getNumParticles());
    drawTarget = GL_POINTS;
}

void PPointGraphicsComponent::tick() {
//updates displacement, duration basically
    std::vector<float> v;  
    int n = static_cast<ParticleSystem*>(actor)->getNumParticles();
    auto particlesystem = static_cast<ParticleSystem*>(actor);
    Particle* p = particlesystem->getParticles();
    float maxDuration = 1.0;
    if (auto prc = particlesystem->getComponent<PRefreshComponent>()) {
        maxDuration = prc->getParticleLifetime();
    }
    v.resize(n*4);
    for (int i = 0; i < n; i++) {
        v[i] = p[i].posVec.x; 
        i++;  
        v[i] = p[i].posVec.y;
        i++;
        v[i] = p[i].posVec.z;
        i++;
        v[i] = p[i].duration / (maxDuration) ;
    }//may need optimization contigious vs separated memory
    updateInstanceBuffer(v);
}
 
