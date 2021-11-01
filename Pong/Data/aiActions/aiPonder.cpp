//
//  aiPonder.cpp
//  Pong
//
//  Created by Joseph Gu on 10/16/21.
//

#include "aiPonder.hpp"
#include "NameComponent.hpp"

aiPonder::aiPonder(float i) : aiAction(i) {
    myWatch.resetTime();
}

void aiPonder::tick(Actor *actor) { 
    float t = myWatch.getTime();
    if (t < 6.0) {
        actor->rotate(glm::vec3(0, 3.1, 0));
    } 
    else {
        actor->getComponent<NameComponent>()->speak("Hmmmm...", 2.0);
        myWatch.resetTime();
        interest -= 1.0f;
    }
}  
 

 
