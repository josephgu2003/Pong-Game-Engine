//
//  Particle.hpp
//  Pong
//
//  Created by Joseph Gu on 5/17/21.
//

#ifndef Particle_hpp
#define Particle_hpp

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <random>
#include <algorithm>
#include <map>
#include "AssetManager.hpp"
#include <functional>

struct Particle { // implies a set of vertices, don't store it or too much memory
    glm::vec3 posVec, velVec;
    GLuint texture;
    float duration;
};

class ParticleEffect {
protected:
    float size;
    
    std::vector<Particle> particles;
    
    int cyclecount;
    int numParticles;
    float x,y,z;
    
    int firstUnused;
    int cycle;
    std::map<char, Character> glyphs;
    
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution;
    virtual void refreshParticle();
public:
    void init(float size_, glm::vec3 posVec_, float x_, float y_, float z_, int numParticles_, int cyclecount_);
    float getSize();
    int shader;
    glm::vec3 posVec;
    GLuint texture;
    ParticleEffect();
    ParticleEffect(glm::vec3 posVec_, float x_, float y_, float z_, int numParticles_, int cyclecount_);
    ~ParticleEffect();
    void tick();
    int getNumParticles();
    Particle& getNthParticle(int n);
};

class ComplexParticles : public ParticleEffect {
    void refreshParticle();
public:
    void init(float size_, glm::vec3 posVec_, float x_, float y_, float z_, int numParticles_, int cyclecount_);
};
#endif /* Particle_hpp */
