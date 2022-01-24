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
#include "AIAction.hpp"
#include "aiDance.hpp"

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

enum aiActionEnum {
    ACT_DIALOGUE,
    ACT_PONDER,
    ACT_WATCH_SCENERY,
    ACT_DANCE,
    ACT_SING_FRIEND,
    ACT_GIVE_GIFT, 
};


class CharacterComponent : public NameComponent {
private:
    std::map<std::string, std::shared_ptr<Relationship>> relationships; // other character's name, relationship between
    PersonalityTrait traits[MAX_PTRAITS];
    std::shared_ptr<aiAction> currentAction;
    int nextOpenTrait;
public:
    CharacterComponent(Actor& actor);
    virtual void tick() override; 
    void newRelationship(const std::shared_ptr<Relationship>& rs);
    bool hasRelationshipWith(Actor* actor, RelationShipType rst, Relationship*& r);
    void newTrait(PersonalityTrait pt);
    bool hasTrait(PersonalityTrait pt);
    bool hasRelationshipWith(Actor* actor, RelationShipType rst, float& intensity);
    RelationShipType getRelationshipTypeWith(Actor* actor);
    bool hasCurrentAction();
    void newAction(aiActionEnum action, Actor* target);
    void newAction(const std::shared_ptr<aiAction>& a);
    std::vector<QA> getQAWith(Actor* target);
};

#endif /* CharacterComponent_hpp */
