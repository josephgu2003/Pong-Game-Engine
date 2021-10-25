//
//  aiDance.cpp
//  Pong
//
//  Created by Joseph Gu on 10/9/21.
//

#include "aiDance.hpp"
#include "NameComponent.hpp" 

aiDance::aiDance(float i) : aiAction(i) {
    myWatch.resetTime();
} 
 
void aiDance::tick(Actor *actor) {
    float t = myWatch.getTime();
    
    if (t < 1.0) {
        actor->posRight(0.05); 
        actor->jump();
    } else if (t < 2.0) {
        actor->posDir(0.05); 
    } else if (t < 3.0) {
        actor->posRight(-0.05);
    } else if (t < 4.0) {
        actor->posDir(-0.05);
        actor->jump();
    } else {
        actor->getComponent<NameComponent>()->speak("It is a great day to dance-", 3.0);
        myWatch.resetTime();
        interest -= 2.5f; 
    }
}
 
