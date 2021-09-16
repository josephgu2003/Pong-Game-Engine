//
//  RunicLetters.cpp
//  Pong
//
//  Created by Joseph Gu on 9/13/21.
//

#include "RunicLetters.hpp"

void RunicLetters::setGraphics(Shader& shader_) {
    AssetManager::loadTexture(TEX_PEACH, &texture, false); //***
    textureTarget = GL_TEXTURE_2D; 
    drawTarget = GL_TRIANGLES;
    shader_.init("Shaders/3DParticle.vs", "Shaders/GenericDiffuse.fs");
    shader_.use();
    shader_.setFloat("size", size);
    shader_.setFloat("brightness", 2.0);
} 
  
void RunicLetters::refreshParticle() {
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
particles[firstUnused].velVec = (glm::vec3(a-0.5,0.2*b-0.7,c-0.5));
    particles[firstUnused].pyrAngles += glm::vec3(0,20.0*b,0);
    particles[firstUnused].duration = 10; 
particles[firstUnused].texture = texture.id;
}
 
void RunicLetters::tick() {
    ParticleEffect::tick();
    for (int i = 0; i < numParticles; i++) {
        particles[i].pyrAngles += glm::vec3(0, 2, 0);
    }
}
