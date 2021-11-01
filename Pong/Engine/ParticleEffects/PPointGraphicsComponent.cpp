//
//  PPointGraphicsComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 10/21/21.
//

#include "PPointGraphicsComponent.hpp"
#include "VertexLoader.hpp"

PPointGraphicsComponent::PPointGraphicsComponent(ParticleSystem& pe, int numberParticles, float size, Shader* shader, const TextureMaps& map_) : PGraphicsComponent(pe, size, shader, map_){
    int datasize = numberParticles * 4 * sizeof(float);
    std::vector<int> v = {3,1};
    VertexLoader::loadPoint(VAO, VBO, EBO, numIndices);
    makeInstanceBuffer(datasize, 2, v, actor->getNumParticles());
    drawTarget = GL_POINTS;
}

void PPointGraphicsComponent::tick() {
//updates displacement, duration basically
    std::vector<float> v;
    int n = actor->getNumParticles();
    Particle* p = actor->getParticles();
    v.resize(n*4);
    for (int i = 0; i < n; i++) {
        v[i] = p[i].posVec.x;
        i++;  
        v[i] = p[i].posVec.y;
        i++;
        v[i] = p[i].posVec.z;
        i++;
        v[i] = p[i].duration;
    }//may need optimization contigious vs separated memory
    updateInstanceBuffer(v);
}
 
