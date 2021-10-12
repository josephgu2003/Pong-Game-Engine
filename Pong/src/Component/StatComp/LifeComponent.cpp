//
//  LifeComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 9/10/21.
//

#include "LifeComponent.hpp"
#define GLFW_DLL
#include <GLFW/glfw3.h>

#define HUNGER_RATE 0.001

LifeComponent::LifeComponent(Actor& actor) : Component(actor) {
    type = LIFE;
}

void LifeComponent::tick() {
    float elapsedT = (float)glfwGetTime();
    if (life.value <= 0.0) {
        //dies 
    }
    if (hunger.value > 0) {
        hunger.value -= elapsedT * HUNGER_RATE;
    } else {
        life.value -= elapsedT * HUNGER_RATE;
    }
    if (stamina.value > 0) {
        stamina.value -= elapsedT * HUNGER_RATE;
    } else {
        life.value -= elapsedT * HUNGER_RATE;
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

void LifeComponent::incLife(float increment) {
    float newLife = life.value + increment;
    if (newLife < life.max) {
        life.value = newLife;
    } else if (newLife >= life.max) {
        life.value = life.max;
    }
}
