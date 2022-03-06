//
//  SwordWorld.cpp
//  Pong
//
//  Created by Joseph Gu on 12/17/21.
//

#include "SwordWorld.hpp"
#include "ParticleFactory.hpp"
#include "Actor.hpp"
#include "AnimComponent.hpp"
#include "World.hpp"  

void SwordWorld::call() {
    ParticleFactory pf; 
    std::shared_ptr<ParticleSystem> swords = pf.makeParticles(PE_MIST, actor->getPos()+glm::vec3(0, -3.0f, 0));
    swordsRef = swords;  
    world->insert<ParticleSystem>(swords);   
    if (auto ac = actor->getComponent<AnimComponent>()) {
        ac->playAnim("DrawWeapon",false);
    } 
}
void SwordWorld::tick() {
    duration -= (float)glfwGetTime();
    if (duration < 0.0) on = false; 
} 

SwordWorld::~SwordWorld() {
    if (auto x = swordsRef.get()) {
        world->deleteX<ParticleSystem>(x);
    }
} 
  
