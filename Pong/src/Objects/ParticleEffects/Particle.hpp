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
    float friction;
    bool useTexture = true;
    float size;
    
    std::vector<Particle> particles;
    
    Actor* actor = NULL;
    
    int numParticles;
    float x,y,z;
    
    int firstUnused;
    Shader shader;

    float ptcPerSec;
    float duration;
    float timer = 0;
    
    glm::vec3 force = glm::vec3(0,0,0);
    
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution;
    virtual void refreshParticle() = 0;
public:
    virtual void init(float size_, glm::vec3 posVec_, glm::vec3 dimensions, int numParticles_, float ptcPerSec, float duration, float friction);
    
    virtual void setGraphics() = 0;
    
    void setActor(Actor* actor);
    float getSize();
    
    glm::vec3 posVec;
    GLuint texture;
    GLenum textureTarget;
    GLenum drawTarget;
    int verticesPerDraw;
    
    Shader& getShader();
    
    ParticleEffect();

    ~ParticleEffect();
    virtual void tick();
    
    int getNumParticles();
    Particle& getNthParticle(int n);
    void setForce(glm::vec3 force_);
};

#endif /* Particle_hpp */
