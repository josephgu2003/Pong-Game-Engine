//
//  aiDialogueAction.hpp
//  Pong
//
//  Created by Joseph Gu on 10/26/21.
//

#ifndef aiDialogueAction_hpp
#define aiDialogueAction_hpp

#include <stdio.h>
#include "AIAction.hpp"
#include "Watch.hpp"
#include "Relationship.hpp"

class aiDialogueAction: public aiAction {
private:
    Watch myWatch;
    Actor* user = NULL;
    Actor* target = NULL;
    bool waiting = true;
    std::string nextAns;
    std::vector<QA> questionsForTarget;
    // list of viable questions to ask target, answers are up to ai or player
public:
    aiDialogueAction(float i, Actor* actor, Actor* target);
    virtual void tick(Actor* actor) override;
    void askQuestion(std::string q);
    std::vector<QA> getQuestionsForTarget();
};
 

#endif /* aiDialogueAction_hpp */
