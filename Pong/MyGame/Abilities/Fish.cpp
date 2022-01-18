//
//  Fish.cpp
//  Pong
//
//  Created by Joseph Gu on 6/22/21.
//

#include "Fish.hpp"
#include "Game.hpp"
#include "CombatComponent.hpp"
#include "LifeComponent.hpp"

Fish::Fish(World* world_, Actor* actor_, float duration_) : Ability(world_, actor_, duration_) { 
    fish = factory.makeActor(ACTOR_FISH);
    fish->setPos(actor->getPos() + actor->getDir() + glm::vec3(0,1,0));
}

Fish::~Fish() {
    world->deleteX<Actor>(fish.get());
} 
 
void Fish::call() {

    world->insert<Actor>(fish); 

    dirFired = actor->getDir();
    mode2 = true;

}
 
void Fish::tick() {
    actor->getComponent<LifeComponent>()->incStatValue(-0.03, STAT_ENERGY);
    if (step == 0) {
        if (duration > 16.5) { 
         //   quad.alpha = (18.0-duration)/1.5;
        } 
        else {
            step++;
            freeActor();
        }
    }
     
    if (step == 1) {
        glm::vec3 transform = glm::vec3(0.0f);
        
        float k = (float)(sin(duration*3.0f)-sin(duration*3.0f+0.1f))/(1.0f+(sin(duration*3.0f)*sin(duration*3.0f+0.1f)));
        
        transform.y = (50.0f*atan(k));
        
        float l =  (float)(sin(duration*30.0f)-sin(duration*30.0f+0.1f))/(1.0f+(sin(duration*30.0f)*sin(duration*30.0f+0.1f)));
        
        transform.y += (0.8f*atan(k)+0.2f*50*atan(l)+2.0f);
        transform.y *= 1.8f;

        fish->rotate(glm::vec3(0,transform.y,0)); 
        fish->posRight(-0.08);
    }
    if (duration < 0) {
        on = false;
    }
    duration -= (float)glfwGetTime();
} 

void Fish::freeActor() {
    if (actor->getComponent<CombatComponent>()->getAffecting().lock().get() != NULL) {
       actor->getComponent<CombatComponent>()->getAffecting().lock()->dispel();
    }
}
 
  
 
