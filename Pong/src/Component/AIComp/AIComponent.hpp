//
//  AIComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 9/29/21.
//

#ifndef AIComponent_hpp
#define AIComponent_hpp

#include <stdio.h>
#include "Component.hpp"
#include "aiDance.hpp"
#include "Dice.hpp"
#include "AIAction.hpp"
#include <memory>
#include "CharacterComponent.hpp"

#define MAX_TRAITS 3
// chooses what action to do next, and executes per tick

enum aiActionEnum {
    ACT_WATCH_SCENERY,
    ACT_DANCE
};

struct aiActionAffinity {
    aiActionEnum action;
    float baseProb;
    PersonalityTrait affinities[MAX_TRAITS];
    aiActionAffinity(aiActionEnum action_,
                     float baseProb_,
                     const PersonalityTrait affinities_[MAX_TRAITS]) {
        action = action_;
        baseProb = baseProb_;
        for (int i = 0; i < MAX_TRAITS; i++) {
            affinities[i] = affinities_[i];
        }
    }
};

const static PersonalityTrait aiWatchSceneryTraits[MAX_TRAITS] = {PT_REFLECTIVE, PT_EMPTY, PT_EMPTY};

const static PersonalityTrait aiDanceTraits[MAX_TRAITS] = {PT_ENERGETIC, PT_ROMANTIC, PT_EMPTY};

const static aiActionAffinity aiActionTable[] = {
    aiActionAffinity(ACT_WATCH_SCENERY, 0.1f, aiWatchSceneryTraits),
    aiActionAffinity(ACT_DANCE, 0.1f, aiDanceTraits)
};


class AIComponent : public Component {
private:
    std::map<aiActionEnum, float> probMap; // this way we have stored the probabilities for ez access, separate this from the affinity data
    Dice dice;
    std::unique_ptr<aiAction> currentAction;
    void calcNextAction(Actor& actor);
    void calcProbabilities(Actor& actor);
public:
    AIComponent(Actor& actor);
    virtual void tick() override;
};

// variables affecting choice of action:
// personality
// environment
// people around
// dice roll
 
// passive mode:
// pace and observe nature
// lie down

// alone: need separation between alone and passive so passive actions can be done
// over and over without being wierd, alone is more active
// dance
// sing

// with friend:
// hug
// sing for them
// share a snack

// with enemy:
// threaten
// stand-off (possible escalation into fight)
// ??

// also the leave action

// an action goes on until its done


#endif /* AIComponent_hpp */
