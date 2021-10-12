//
//  CharacterComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 9/22/21.
//

#ifndef CharacterComponent_hpp
#define CharacterComponent_hpp

#include <stdio.h>
#include <map>
#include "NameComponent.hpp"
#include <memory>
#include "Relationship.hpp"

#define MAX_PTRAITS 3

// handles relationships
enum PersonalityTrait { // trait and opposite trait next index
    PT_AMIABLE,
    PT_HATEFUL,
    PT_ENERGETIC,
    PT_LAZY,
    PT_LOYAL,
    PT_DISLOYAL,
    PT_HONEST,
    PT_DISHONEST,
    PT_ROMANTIC,
    PT_UNROMANTIC,
    PT_BRAVE,
    PT_COWARDLY,
    PT_VIOLENT,
    PT_PACIFIST,
    PT_REFLECTIVE,
    PT_SIMPLETON,
    PT_EMPTY //empty trait
};

class CharacterComponent : public NameComponent {
private:
    std::map<std::string, std::weak_ptr<Relationship>> relationships; // other character's name, relationship between
    PersonalityTrait traits[MAX_PTRAITS];
    int nextOpenTrait;
public:
    CharacterComponent(Actor& actor);
    virtual void tick() override;
    void newRelationship(const std::shared_ptr<Relationship>& rs);
    void newTrait(PersonalityTrait pt);
};

#endif /* CharacterComponent_hpp */
