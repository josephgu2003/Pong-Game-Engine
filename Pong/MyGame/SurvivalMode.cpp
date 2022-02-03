//
//  SurvivalMode.cpp
//  Pong
//
//  Created by Joseph Gu on 1/25/22.
//

#include "SurvivalMode.hpp"
#include "World.hpp"
#include "NotificationSystem.hpp"
#include "CombatComponent.hpp"
#include "ChargedSlash.hpp"
#include "LifeComponent.hpp"

SurvivalMode::SurvivalMode(World& w, float radius) : Component(w) {
    dice.setRange(0, 500);
    numberHostiles = 1;
    spawnRadius = radius;
}

bool SurvivalMode::checkHuntersFree() {
    while (!sentHunters.empty()) {
        auto aRef = sentHunters.front();
        
        if (aRef.lock()) return false;
        else sentHunters.pop();
    }
    return true; 
}

void SurvivalMode::tick() {
    if (watch.getTime() > 30.0f && checkHuntersFree()) {
        World* w =  static_cast<World*>(actor);
        Actor* ph = w->getPlayerHero();
        glm::vec3 pos = ph->getPos();
        w->getComponent<NotificationSystem>()->newNotification("New enemies are near...", 4.0f);
        for (int i = 0; i < numberHostiles; i++) {
            std::shared_ptr<Actor> newHostile = maf.makeActor(ACTOR_SCARF_CHAR);
            newHostile->setPos(randomPos(pos));
            newHostile->addComponent<Hunter>(*newHostile.get());
            newHostile->addComponent<CombatComponent>(*newHostile.get());
            newHostile->getComponent<LifeComponent>()->init(100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f);
            
            std::weak_ptr<Actor> aRef = newHostile;
            sentHunters.push(aRef);
            
            w->insert(newHostile);
        }
        watch.resetTime();
    }
} 

glm::vec3 SurvivalMode::randomPos(const glm::vec3& origin) {
    float radians = 2.0f * 3.1415f * dice.roll() / 500.0f;
    
    glm::vec3 newPos = glm::vec3(0.0f);
    
    newPos.x = sin(radians) * spawnRadius + origin.x;
    newPos.y = origin.y;
    newPos.z = cos(radians) * spawnRadius + origin.z;
    
    return newPos;
}

void Hunter::tick() {
    
    Actor* a = static_cast<Actor*>(actor);
    World* w = &(a->getWorld());
    
    if (a->getComponent<LifeComponent>()->getStat(STAT_LIFE).value <= 0.0f) {
        w->deleteX(a);
        return;
    }
    
    if (!w) return;
    Actor* ph = w->getPlayerHero();
    
    if (!ph) return;
    
    if (a->getDistanceTo(ph) > 6.0f) {
        a->orientYawTo(ph);
        a->posDir(0.3);
    } else {
        a->posRight(0.3);
        a->orientYawTo(ph);
        
        if (watch.getTime() > 2.0f) {
        a->getComponent<CombatComponent>()->newAbility<ChargedSlash>(w, a, 2.0f);
            watch.resetTime();
        }
    }
}
 
  
