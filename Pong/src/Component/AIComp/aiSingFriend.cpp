//
//  aiSingFriend.cpp
//  Pong
//
//  Created by Joseph Gu on 10/17/21.
//

#include "aiSingFriend.hpp"
#include "NameComponent.hpp"

aiSingFriend::aiSingFriend(float i, Actor* a) : aiAction(i) {
    myWatch.resetTime();
    target = a;
}

void aiSingFriend::tick(Actor *actor) {
    float t = myWatch.getTime();
    if (t < 4.0) {
    }
    else {
        actor->getComponent<NameComponent>()->speak("I am singing for you," + target->getComponent<NameComponent>()->getCommonName() + "!", 3.0);
        myWatch.resetTime();
        interest -= 1.0f;
    }
    actor->translatePos(0.05f*(target->getPos() - actor->getPos()));
    actor->turnTowards(target->getPos() - actor->getPos());
} 
    

 
