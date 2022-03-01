//
//  ChargedSlash.cpp
//  Pong
//
//  Created by Joseph Gu on 1/4/22.
//

#include "ChargedSlash.hpp"
#include "AnimComponent.hpp"
#include "Actor.hpp"
#include "PropFactory.hpp"
#include "World.hpp"
#include "ParticleFactory.hpp"
#include "PRefreshComponent.hpp"
#include "LifeComponent.hpp"
#include "LifeTime.hpp"

void ChargedSlash::call() {
    actor->getComponent<AnimComponent>()->playAnim("Slash", false);
    
    PropFactory pf;
    std::shared_ptr<Prop> slash = pf.makeProp(PROP_SWORD_SLASH);
    slash->setPos(actor->getPos()+actor->getDir()+glm::vec3(0,0,0));
    slash->orientYawTo(actor->getDir()); 
    slashRef = std::weak_ptr<Prop>();
    slashRef = slash;
    world->insert<Prop>(slash);
    
    partRef = std::weak_ptr<ParticleSystem>();
      
    CollisionCallback cback = [&] (CollisionComponent* me, CollisionComponent* collided) {
        Componentable* target = collided->getParentObject();
        if (actor == target) return;
        if (LifeComponent* lc = target->getComponent<LifeComponent>()) {
            lc->incStatValue(-20.0f, STAT_LIFE);
        };  
            
      /**  ParticleFactory pf_; // WTF??? dumb
        std::shared_ptr<ParticleSystem> part = pf_.makeParticles(PE_SWORD_PARTICLES, glm::vec3(0));
        part->setPos(dynamic_cast<Positionable*>(target)->getPos() + glm::vec3(0,1.0f,0));
        partRef = part; 
        part->getComponent<PRefresh>()->refreshAll();
        world->insert<ParticleSystem>(part);**/ 
           
        PropFactory props;
        auto swordexp = props.makeProp(PROP_SWORD_EXP); 
        swordexp->orientYawTo(slashRef.lock()->getDir());
        swordexp->setPos(dynamic_cast<Positionable*>(target)->getPos() + glm::vec3(0,0.5f,0));
        swordexp->addComponent<LifeTime<Prop>>(*swordexp.get(), 0.3);
        world->insert<Prop>(swordexp);
           
        world->deleteX(slashRef.lock().get());
    };  
    slash->getComponent<CollisionComponent>()->setOnCollision(cback);
}
    
void ChargedSlash::tick() { 
    if (auto slash = slashRef.lock()) {
        slash->posDir(0.5f); 
    }

    duration -= (float)glfwGetTime();
    if (duration < 0.0) on = false;
} 
   
ChargedSlash::~ChargedSlash() {
    if (auto slash = slashRef.lock()) { 
        world->deleteX<Prop>(slash.get());
    }
      
    if (auto part = partRef.lock()) {
        world->deleteX<ParticleSystem>(part.get());
    }
    
}
 
