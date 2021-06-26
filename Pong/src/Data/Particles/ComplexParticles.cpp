//
//  ComplexParticles.cpp
//  Pong
//
//  Created by Joseph Gu on 6/21/21.
//

#include "ComplexParticles.hpp"

void ComplexParticles::refreshParticle() {
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
    //int b = std::rand()%100;
    float c = 0.01*(std::rand()%100);
  //  if (a%2 ==1) a = a*(-1);
   // b = b*(-1);
 //   c = c*(-1);
particles[firstUnused].velVec = (glm::vec3(0.2*a-0.1,-1.0,0.2*c-0.1))+force;
particles[firstUnused].duration = 5;
//particles[firstUnused].texture = textures.at((firstUnused%25)+97);
    particles[firstUnused].texture = texture;
}

void ComplexParticles::init(float size_, glm::vec3 posVec_, float x_, float y_, float z_, int numParticles_, float ptcPerSec_, float duration_) {
    load3DTexture("Resources/Particles/Inky/GWtc0.png", texture);
    ParticleEffect::init(size_, posVec_, x_, y_, z_, numParticles_, ptcPerSec_);
    std::map<char,Character> glyphs;
    loadGlyphs("Resources/Glyphs/minecraft-enchantment.ttf", glyphs);
    textureTarget = GL_TEXTURE_2D_ARRAY;
    shader.init("Shaders/GlyphParticleVShader.vs", "Shaders/GlyphParticleFShader.fs");
    
    extern GLuint uboViewProj;
    glBindBuffer(GL_UNIFORM_BUFFER, uboViewProj);
    GLuint viewproj  = glGetUniformBlockIndex(shader.ID, "ViewProj");
    glUniformBlockBinding(shader.ID, glGetUniformBlockIndex(shader.ID, "ViewProj"), 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboViewProj);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

