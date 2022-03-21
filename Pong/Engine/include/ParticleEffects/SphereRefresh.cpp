//
//  SphereRefresh.cpp
//  Pong
//
//  Created by Joseph Gu on 1/20/22.
//

#include "SphereRefresh.hpp"
#include "Particle.hpp"

SphereRefresh::SphereRefresh(ParticleSystem& pe, float particleMaxDuration_, int ptcPerSecond, float refreshInterval, float radius_, float minSpeed_, float maxSpeed_) : PRefresh(pe, particleMaxDuration_, ptcPerSecond, refreshInterval) {
    radius = radius_;
    minSpeed = minSpeed_;
    maxSpeed = maxSpeed_; 
}
void SphereRefresh::refreshParticle() {
    glm::vec3 displacement = glm::vec3(0.0f,0.0f,0.0f);
    
    auto randomAngleZeroToTwoRadians = [&] () {
        return 2.0f*3.1415f*(mainDice.roll() % 500)/300.0f;
    }; 
    
    displacement.y = sin(randomAngleZeroToTwoRadians());
    float xzSeed = randomAngleZeroToTwoRadians();
    float newHypo = sqrt(1.0f-pow(displacement.y,2));
    displacement.x = std::sin(xzSeed) * newHypo;
    displacement.z = std::cos(xzSeed) * newHypo;
       
    if (refreshVel && velocities) {
        velocities[firstUnused] = displacement*(minSpeed + (maxSpeed-minSpeed)*((mainDice.roll() % 100) / 100.0f));
    }
    
    displacement *= radius * (mainDice.roll() % 500) /500.0f;
    
    particles[firstUnused].posVec = particleOrigin + displacement;
    
    particles[firstUnused].pyrAngles = glm::vec3(0,(mainDice.roll() % 180),0);
     
    particles[firstUnused].duration = particleMaxDuration;
     
    if(firstUnused == (numParticles-1)) {
        firstUnused = 0;
    } else if (particles[firstUnused+1].duration<=0) {
        firstUnused++;
    } else {
        
    }
}

