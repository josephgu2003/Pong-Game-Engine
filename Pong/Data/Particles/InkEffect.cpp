//
//  InkGlyphs.cpp
//  Pong
//
//  Created by Joseph Gu on 6/13/21.
//

#include "InkEffect.hpp"



void InkEffect::setGraphics(Shader& shader) {
    load3DTexture("Resources/Particles/Smokes/smoke000.jpg", texture);
    drawTarget = GL_TRIANGLES;
     textureTarget = GL_TEXTURE_2D_ARRAY;
     shader.init("Shaders/SmokeVShader.vs", "Shaders/SmokeFShader.fs");
}

void InkEffect::refreshParticle() {
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

particles[firstUnused].velVec = (glm::vec3(2*a,b-0.5,c));
particles[firstUnused].duration = 3.0f;
particles[firstUnused].texture = texture;
}
