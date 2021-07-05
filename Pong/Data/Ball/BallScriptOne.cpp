//
//  BallScriptOne.cpp
//  Pong
//
//  Created by Joseph Gu on 7/2/21.
//

#include "World.hpp"
#include "BallScriptOne.hpp"
#include "Dialogue.hpp"
#include "FallingLetters.hpp"
//#include "Actor.hpp"
#include "Speech.hpp"

void BallScriptOne::tick() {
    switch (state) {
        case STATE_PASSIVE: {
            if (actor->biggestTarget != NULL) {
                state = STATE_AGGRESSIVE;
            }
        };
            
        case STATE_AGGRESSIVE:
            if (actor->biggestTarget != NULL) {
        actor->turnTowards(actor->biggestTarget->getPos()-actor->getPos());
                
        if (step == 0) {
            Dialogue* dialogue = new Dialogue(&actor->getWorld(), actor, 6);
            actor->abilityQ.push_back(dialogue);
            currentAbility = dialogue;
            step++;
            return;
        }
        
        if (step == 1) {
            if (currentAbility->on == false) {
                step++;
                currentAbility = NULL;
            FallingLetters* letters = new FallingLetters(&actor->getWorld(), actor, 5);
            letters->setTarget(actor->biggestTarget);
            actor->abilityQ.push_back(letters);
            currentAbility = letters;
            actor->biggestTarget->affecting = letters;
            }
            return;
        }
                
            if (step == 2) {
                if (currentAbility->on == false) {
                    step++;
                    currentAbility = NULL;
                    std::vector<std::string> strings = {"Then go"};
                    Speech* speech = new Speech(&actor->getWorld(), actor, 5, strings);
                    actor->abilityQ.push_back(speech);
                    currentAbility = speech;
                }
                return;
            }
                
            break;

            }
    }
            
}

void BallScriptOne::init() {
    
}
