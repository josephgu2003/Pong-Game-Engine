//
//  LifeComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 9/10/21.
//

#ifndef LifeComponent_hpp
#define LifeComponent_hpp

#include <stdio.h>
#include "Component.hpp"
#include "Subject.hpp"

struct Stat {
    float value;
    float max;
};

enum StatType {
    STAT_LIFE,
    STAT_HUNGER,
    STAT_STAMINA,
    STAT_ENERGY
};

// survival aspect, deals with staying alive

class Actor;

class LifeComponent : public Component, public Subject {
private:
    Stat life; //hp, life stat, healthiness, affected by illness poison mental stuff
    Stat hunger; //necessitates eating and drinking, depletes over time
    Stat stamina; //used for running and physical abilties, replenished over time and by sleeping, also slowly depletes
    Stat energy; //casting resource for spells, replenished over time and sleeping
    Stat& getStatRef(StatType st);
public:
    LifeComponent(Actor& actor);
    virtual void tick() override;
    void init(float life, float maxlife, float hunger, float maxhunger, float stamina, float maxstamina, float energy, float maxenergy);
    Stat getStat(StatType type) const;
    void incStatValue(float increment, StatType st); 
};

#endif /* LifeComponent_hpp */
 
