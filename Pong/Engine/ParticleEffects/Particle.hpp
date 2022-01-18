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
#include "Positionable.hpp"
#include "Componentable.hpp"
#include "Watch.hpp"

struct Particle {
    glm::vec3 posVec = glm::vec3(0);
    glm::vec3 pyrAngles = glm::vec3(0);
    float duration = 0.0;
};

struct Character;

class World;

class Shader;  

class ParticleSystem : public Positionable, public Componentable {
protected:
  //  std::vector<Particle> particles;
    World* world = NULL;
    
    int numParticles;
    
    Watch watch;

    float duration;
    
    virtual void refreshParticle() {}
    Particle* particles = NULL; 
public:
    void setWorld(World* world);  
    
    ParticleSystem(int numParticles, float duration);

    virtual ~ParticleSystem();
    virtual void tick();
    
    int getNumParticles();
    Particle* getParticles();
    Particle& getNthParticle(int n);
};
/**
glm::vec3 force = glm::vec3(0,0,0);
void setForce(glm::vec3 force_);
std::vector<Force*> forces;
float friction;
bool useTexture = true;
float size;

, velVec, pyrAngles;
GLuint texture;
float duration;**/

// particle effect modules:
// physics/ force
// graphics
// refresh comp
//trail

// exmaple firework
// graphics : comp for point graphics, use no rotation data
// expects uniforms of color and updating the instancing buffer with positions
// and lifetimes
// physics : move the particles in explosion
// trail : add trail particles ?? how ?? trail can be a separate effect but wierd
// lifetime and refresh : refresh particles differently trail and not trail??



#endif /* Particle_hpp */
