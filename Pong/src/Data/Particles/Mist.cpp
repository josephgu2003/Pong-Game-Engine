//
//  Leaves.cpp
//  Pong
//
//  Created by Joseph Gu on 6/10/21.
//

#include "Mist.hpp"
#include "AssetManager.hpp"

void Mist::init(float size_, glm::vec3 posVec_, float x_, float y_, float z_, int numParticles_, int cyclecount_) {
    size = size_;
    std::srand(314159);
    posVec = posVec_;
    texture = loadTexture(TEX_MIST); //***
    textureTarget = GL_TEXTURE_2D;
    x = x_;
    y = y_;
    z = z_;
    numParticles = numParticles_;
    for (int i = 0; i < numParticles; i++)
        particles.push_back(Particle());
    cycle = cyclecount_;
    cyclecount = cyclecount_;
    firstUnused = 0;
    distribution = std::uniform_int_distribution<int>(1,1000);
    
    shader.init("Shaders/ParticleVertexShader.vs", "Shaders/ParticleFragmentShader.fs");
    
    extern GLuint uboViewProj;
    glBindBuffer(GL_UNIFORM_BUFFER, uboViewProj);
    GLuint viewproj  = glGetUniformBlockIndex(shader.ID, "ViewProj");
    glUniformBlockBinding(shader.ID, glGetUniformBlockIndex(shader.ID, "ViewProj"), 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboViewProj);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Mist::tick() {
    if (actor != NULL) {
    posVec = actor->getPos();
    }
    
    float dt = glfwGetTime();
    if (cycle == cyclecount) {
        cycle = 0;
            
            for (int i = 0; i < 3; i++) {
            refreshParticle();
            if(firstUnused == (particles.size()-1)) {
                firstUnused = 0;
            }
            else if(particles[firstUnused+1].duration<=0) firstUnused++;
        }
    }
    cycle++;
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
  //  if (a%2 ==1) a = a*(-1);
   // b = b*(-1);
 //   c = c*(-1);
particles[firstUnused].velVec = (glm::vec3(0.1*a-0.05,0.2*b-0.1,0.1*c-0.05));
particles[firstUnused].duration = 15;
particles[firstUnused].texture = texture;
}
