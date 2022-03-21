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
#include "Renderer.hpp" 

PPointGraphicsComponent::PPointGraphicsComponent(ParticleSystem& pe, int numberParticles, float size_, Shader* shader, const Material& map_) : GraphicsComponent(pe, shader, map_, DRAW_TRANSPARENT){
    
    shader->use();
    shader->setUniform("size", size_);
    Renderer::bindShaderUniblock(shader, ViewProj);
    // need to load model data to vao vbo,
    
    size = size_; 
    
    drawCall = [] (Renderer* r, GraphicsComponent* gc) {
        r->renderParticles(gc);
    };
     
    int datasize = numberParticles * 4 * sizeof(float);
    std::vector<int> v = {3,1};
    VertexLoader::loadPoint(VAO, VBO, EBO, numIndices);
    makeInstanceBuffer(datasize, 2, v, static_cast<ParticleSystem*>(actor)->getNumParticles()); // should be 3 not 2 but it is corrected in vertex shader
    drawTarget = GL_POINTS;
}

void PPointGraphicsComponent::tick() {
//updates displacement, duration basically
    GraphicsComponent::tick(); 
    std::vector<float> v;   
    int n = static_cast<ParticleSystem*>(actor)->getNumParticles();
    auto particlesystem = static_cast<ParticleSystem*>(actor);
    Particle* p = particlesystem->getParticles();
    float maxDuration = 1.0;
    if (auto prc = particlesystem->getComponent<PRefreshComponent>()) {
        maxDuration = prc->getParticleLifetime();
    }
    v.resize(n*4);
    
    int counter = 0;
    for (int i = 0; i < n; i++) { 
        v[counter] = p[i].posVec.x;
        counter++;
        v[counter] = p[i].posVec.y;
        counter++;
        v[counter] = p[i].posVec.z;
        counter++;
        v[counter] = p[i].duration / (maxDuration);
        counter++;
    }//may need optimization contigious vs separated memory
    updateInstanceBuffer(v);
} 
 
PPointGraphicsComponent::~PPointGraphicsComponent() {
    
}

