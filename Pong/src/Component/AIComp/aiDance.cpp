//
//  aiDance.cpp
//  Pong
//
//  Created by Joseph Gu on 10/9/21.
//

#include "aiDance.hpp"


aiDance::aiDance() {
    myWatch.resetTime();
}

void aiDance::tick(Actor *actor) {
    float t = myWatch.getTime();
    
    if (t < 1.0) {
        actor->posRight(0.05);
    } else if (t < 2.0) {
        actor->posDir(0.05);
    } else if (t < 3.0) {
        actor->posRight(-0.05);
    } else if (t < 4.0) {
        actor->posDir(-0.05);
    } else {
        myWatch.resetTime();
    }
}
