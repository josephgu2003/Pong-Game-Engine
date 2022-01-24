//
//  aiDialogueAction.cpp
//  Pong
//
//  Created by Joseph Gu on 10/26/21.
//

#include "aiDialogueAction.hpp"
#include "CharacterComponent.hpp"

aiDialogueAction::aiDialogueAction(float i, Actor* actor_, Actor* target_) : aiAction(i) {
    target = target_;
    user = actor_;
    questionsForTarget = user->getComponent<CharacterComponent>()->getQAWith(target);
    // list of possible questions and answers are obtained
}

void aiDialogueAction::tick(Actor *actor) { 
    if (!waiting) {
        if (myWatch.getTime() > 3.0) {
            target->getComponent<NameComponent>()->speak(nextAns, 3.0);
            waiting = true;
            nextAns = ""; 
        }
    }
}
 
void aiDialogueAction::askQuestion(std::string question) {
    user->getComponent<NameComponent>()->speak(question, 3.0);
    myWatch.resetTime();
    waiting = false;
    for(auto i : questionsForTarget) {
        if (i.question == question) {
            nextAns = i.answer;
        }
    }
}
 
std::vector<QA> aiDialogueAction::getQuestionsForTarget() {
    return questionsForTarget;
}
