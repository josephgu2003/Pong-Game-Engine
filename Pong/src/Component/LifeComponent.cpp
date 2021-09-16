//
//  LifeComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 9/10/21.
//

#include "LifeComponent.hpp"

LifeComponent::LifeComponent(Actor& actor) : Component(actor) {
    type = LIFE;
}

void LifeComponent::tick() {
    if (life.value <= 0.0) {
        //dies 
    }
    
}

void LifeComponent::init(float life_, float maxlife, float hunger_, float maxhunger, float stamina_, float maxstamina, float energy_, float maxenergy) {
    life.value = life_;
    life.max = maxlife;
    hunger.value = hunger_;
    hunger.max = maxhunger;
    stamina.value = stamina_;
    stamina.max = maxstamina;
    energy.value = energy_;
    energy.max = maxenergy;
}

Stat LifeComponent::getStat(StatType type) {
    switch (type) {
        case STAT_LIFE:
            return life;
            break;
            
        case STAT_HUNGER:
            return hunger;
            break;
            
        case STAT_STAMINA:
            return stamina;
            break;
            
        case STAT_ENERGY:
            return energy;
            break;
    }
}
