//
//  ScriptOne.cpp
//  Pong
//
//  Created by Joseph Gu on 7/9/21.
//

#include "ScriptOne.hpp"
#include "Game.hpp"
#include "Dialogue.hpp"
#include "FallingLetters.hpp"
#include "Speech.hpp"

void ScriptOne::tick() {
    switch (state) {
        case STATE_PASSIVE: {
            if (ball->biggestTarget != NULL) {
                state = STATE_AGGRESSIVE;
            }
        };
            
        case STATE_AGGRESSIVE:
            if (ball->biggestTarget != NULL) {
        ball->turnTowards(ball->biggestTarget->getPos()-ball->getPos());
                
        if (step == 0) {
            Dialogue* dialogue = new Dialogue(world, ball, 6);
            ball->abilityQ.push_back(dialogue);
            currentAbility = dialogue;
            step++;
            return;
        }
                
        if (step == 1) {
            if (currentAbility->on == false) {
                step++;
                currentAbility = NULL;
            FallingLetters* letters = new FallingLetters(world, ball, 5);
            letters->setTarget(ball->biggestTarget);
            ball->abilityQ.push_back(letters);
            currentAbility = letters;
            ball->biggestTarget->affecting = letters;
            }
            return;
        }
                
            if (step == 2) {
                if (currentAbility->on == false) {
                    step++;
                    currentAbility = NULL;
                    std::vector<std::string> strings = {"Then go"};
                    Speech* speech = new Speech(world, (ball), 5,strings);
                    ball->abilityQ.push_back(speech);
                    currentAbility = speech;
                }
                return;
            }
                
            break;

            }
    }
}

void ScriptOne::init(Game* game) {
    pHero = static_cast<PlayerHero*>(game->getNumberable(2));
    ball = static_cast<Ball*>(game->getNumberable(1));
    world = static_cast<World*>(game->getNumberable(0));
}
