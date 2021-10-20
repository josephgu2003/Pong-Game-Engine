//
//  Particle.hpp
//  Pong
//
//  Created by Joseph Gu on 5/17/21.
//

#ifndef Particle_hpp
#define Particle_hpp

#include <glm/glm.hpp>
#include <vector>
#include <random>
#include <algorithm>
#include <map>
#include <functional>
#include "Actor.hpp"
#include "Shader.hpp"
#include "GraphicsComponent.hpp" 
#include "Force.hpp"
#include "Watch.hpp"
#include "Positionable.hpp"
 
struct Particle { // implies a set of vertices, don't store it or too much memory
    glm::vec3 posVec, velVec, pyrAngles;
    GLuint texture;
    float duration;
};

struct Character;

class World;

class ParticleEffect : public Positionable {
protected:
    float friction;
    bool useTexture = true;
    float size;
    
    std::vector<Particle> particles;
    
    Actor* actor = NULL;
    World* world = NULL;
    
    int numParticles;
    float x,y,z;
    
    int firstUnused;

    float ptcPerSec;
    float duration;
    Watch myWatch;
    
    glm::vec3 force = glm::vec3(0,0,0);
    
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution;
    virtual void refreshParticle() {}
public:
    GraphicsComponent* graphics = nullptr;
    virtual void init(float size_, glm::vec3 posVec_, glm::vec3 dimensions, int numParticles_, float ptcPerSec, float duration, float friction);
    
    virtual void setGraphics(Shader* shader) = 0;
    
    std::vector<Force*> forces;
    virtual void setActor(Actor* actor);
    void setWorld(World* world);
    float getSize();
    
    Texture texture;
    GLenum textureTarget;
    GLenum drawTarget;
    
    ParticleEffect();

    virtual ~ParticleEffect();
    virtual void tick();
    
    int getNumParticles();
    Particle& getNthParticle(int n);
    void setForce(glm::vec3 force_);
};

#endif /* Particle_hpp */
