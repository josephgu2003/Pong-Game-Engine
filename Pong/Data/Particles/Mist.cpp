//
//  Leaves.cpp
//  Pong
//
//  Created by Joseph Gu on 6/10/21.
//

#include "Mist.hpp"
#include "AssetManager.hpp"

void Mist::setGraphics(Shader& shader_) {
    AssetManager::loadTexture(TEX_MIST, &texture, false); //***
    textureTarget = GL_TEXTURE_2D;
    drawTarget = GL_POINTS;
    shader_.init("Shaders/MistVShader.vs", "Shaders/MistFShader.fs");
    shader_.use();
    shader_.setFloat("size", size);
}

void Mist::refreshParticle() {
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
particles[firstUnused].velVec = (glm::vec3(0.1*a-0.05,0.2*b-0.1,0.1*c-0.05));
particles[firstUnused].duration = 30;
particles[firstUnused].texture = texture.id;
}

