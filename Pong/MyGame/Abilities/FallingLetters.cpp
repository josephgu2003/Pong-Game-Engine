//
//  FallingLetters.cpp
//  Pong
//
//  Created by Joseph Gu on 6/2/21.
//

#include "FallingLetters.hpp"
#include "Particle.hpp"
#include <memory>
#include "Force.hpp"
#include "World.hpp"
#include "Actor.hpp"
#include "LifeComponent.hpp"
#include "AnimComponent.hpp"
#include "LifeTime.hpp"
#include "PropFactory.hpp"
#include "MovementController.hpp"

FallingLetters::FallingLetters(World* world_, Actor* actor_, float duration_) : Ability(world_, actor_, duration_)  {
    
}

FallingLetters::FallingLetters(World* world_, Actor* actor_, float duration_, std::weak_ptr<Actor>& target) : Ability(world_, actor_,duration_, target) {
    
}
 
FallingLetters::~FallingLetters() {
    world->deleteX<ParticleSystem>(letters.lock().get());
    if (target.lock()) {
        target.lock()->setState(STATE_IDLE); 
    } 
    if (auto anim = actor->getComponent<AnimComponent>()) {
      //  anim->playAnim("HollowKnight__Armature|Walk");
    }
}  
   
void FallingLetters::call() {
    glm::vec3 pos = glm::vec3(actor->getPos());
    if (auto t = target.lock()) {
        if (auto mc = t->getComponent<MovementController>()) {
            mc->paralyze(duration);
        }
        if (auto ac = t->getComponent<AnimComponent>()) {
            ac->playAnim("Straining", 12, 49);
        }
        pos = t->getPos();   
    }
    if (auto ac = actor->getComponent<AnimComponent>()) {
        ac->playAnim("SlowDraw", false);
    } 
    std::shared_ptr<ParticleSystem> ps = pf.makeParticles(PE_RUNICLETTERS, pos+glm::vec3(0,2,0));
    letters = ps;
    world->insert<ParticleSystem>(ps);

  /**  PropFactory pf;
    auto aurora = pf.makeProp(PROP_IMPRISONMENT);
    aurora->setPos(actor->getPos()); 
    aurora->addComponent<LifeTime<Prop>>(*aurora.get(), 10.0f);
    world->insert<Prop>(aurora);**/
}    
 
void FallingLetters::tick() {   
    duration -= glfwGetTime();
    if (duration < 0) { 
        on = false;
    }
    auto t = target.lock();
    if (t && t->getComponent<LifeComponent>()) { 
        t->getComponent<LifeComponent>()->incStatValue(-0.0005, STAT_LIFE);
    }
}
 
 
