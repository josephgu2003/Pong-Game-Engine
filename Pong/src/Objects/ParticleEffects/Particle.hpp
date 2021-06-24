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
#include "Actor.hpp"
#include "Shader.hpp"

struct Particle { // implies a set of vertices, don't store it or too much memory
    glm::vec3 posVec, velVec;
    GLuint texture;
    float duration;
};

struct Character;

class ParticleEffect {
protected:
    
    float size;
    
    std::vector<Particle> particles;
    
    Actor* actor = NULL;
    
    int cyclecount;
    int numParticles;
    float x,y,z;
    
    int firstUnused;
    int cycle;
    
    glm::vec3 force = glm::vec3(0,0,0);
    
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution;
    virtual void refreshParticle();
public:
    virtual void init(float size_, glm::vec3 posVec_, float x_, float y_, float z_, int numParticles_, int cyclecount_);
    
    void setActor(Actor* actor);
    float getSize();
    
    glm::vec3 posVec;
    Shader shader;
    GLuint texture;
    GLenum textureTarget;
    
    ParticleEffect();
    ParticleEffect(glm::vec3 posVec_, float x_, float y_, float z_, int numParticles_, int cyclecount_);
    ~ParticleEffect();
    virtual void tick();
    
    int getNumParticles();
    Particle& getNthParticle(int n);
    void setForce(glm::vec3 force_);
};

#endif /* Particle_hpp */
