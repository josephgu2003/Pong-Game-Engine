//
//  InkGlyphs.cpp
//  Pong
//
//  Created by Joseph Gu on 6/13/21.
//

#include "InkGlyphs.hpp"

void InkGlyphs::init(float size_, glm::vec3 posVec_, float x_, float y_, float z_, int numParticles_, float ptcPerSec_, float duration) {
    size = size_;
    std::srand(314159);
    posVec = posVec_;
    force = glm::vec3(0,0,0);
    x = x_;
    y = y_;
    z = z_;
    numParticles = numParticles_;
    for (int i = 0; i < numParticles; i++)
        particles.push_back(Particle());
    firstUnused = 0;
    distribution = std::uniform_int_distribution<int>(1,1000);
   load3DTexture("Resources/Particles/Smokes/smoke000.jpg", texture);
   // texture = 0;
  //load3DTexture("Resources/Particles/Ink/Ink/ink000.png", texture);
    textureTarget = GL_TEXTURE_2D_ARRAY;
    shader.init("Shaders/GlyphParticleVShader.vs", "Shaders/GlyphParticleFShader.fs");
    
    extern GLuint uboViewProj;
    glBindBuffer(GL_UNIFORM_BUFFER, uboViewProj);
    GLuint viewproj  = glGetUniformBlockIndex(shader.ID, "ViewProj");
    glUniformBlockBinding(shader.ID, glGetUniformBlockIndex(shader.ID, "ViewProj"), 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboViewProj);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void InkGlyphs::tick() {
    if (actor != NULL) {
    posVec = actor->getPos();
    }
    
    float dt = (float)glfwGetTime();

            
            for (int i = 0; i < 2; i++) {
            refreshParticle();
            if(firstUnused == (particles.size()-1)) {
                firstUnused = 0;
            }
            else if(particles[firstUnused+1].duration<=0) firstUnused++;
        }


    for (int i = 0; i < particles.size(); i++) {
        if (particles[i].duration > 0) {
          //  particles[i].velVec += force;
     //   if (glm::length(particles[i].velVec) < 0.03) particles[i].velVec = glm::vec3 (0,0,0);
            particles[i].posVec.x += particles[i].velVec.x*dt;
            particles[i].posVec.y += particles[i].velVec.y*dt;
            particles[i].posVec.z += particles[i].velVec.z*dt;
            particles[i].duration -= dt;
        }
    }
}

void InkGlyphs::refreshParticle() {
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
    float a = 0.003*(std::rand()%100);
    //int b = std::rand()%100;
    float b = 0.003*(std::rand()%100);
    float c = 0.003*(std::rand()%100);
  //  if (a%2 ==1) a = a*(-1);
   // b = b*(-1);
 //   c = c*(-1);
particles[firstUnused].velVec = (glm::vec3(2*a,b-0.5,c));
particles[firstUnused].duration = 3.0f;
particles[firstUnused].texture = texture;
}
