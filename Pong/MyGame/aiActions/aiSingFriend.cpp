//
//  aiSingFriend.cpp
//  Pong
//
//  Created by Joseph Gu on 10/17/21.
//

#include "aiSingFriend.hpp"
#include "NameComponent.hpp"
#include "CharacterComponent.hpp"
#include "Relationship.hpp"

aiSingFriend::aiSingFriend(float i, Actor* a) : aiAction(i) {
    myWatch.resetTime();
    target = a;
}

void aiSingFriend::tick(Actor *actor) { 
    float t = myWatch.getTime();  
    if (t < 4.0) {
    }
    else {
        std::string n = target->getComponent<NameComponent>()->getName();
        auto x = actor->getComponent<CharacterComponent>();
        if (x) {
            Relationship* r;
            x->hasRelationshipWith(target, RS_FRIEND, r);
            if (r) {
                n = r->getNickName(x->getName());
            }
        }
        actor->getComponent<NameComponent>()->speak("Let me sing for you," + n + "!", 3.0);
        myWatch.resetTime();
        interest -= 1.0f;
    }
    actor->translatePos(0.05f*(target->getPos() - actor->getPos()));
    actor->turnTowards(target->getPos() - actor->getPos());
} 
    

 
