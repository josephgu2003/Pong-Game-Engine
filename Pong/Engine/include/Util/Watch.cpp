//
//  Watch.cpp
//  Pong
//
//  Created by Joseph Gu on 10/7/21.
//

#include "Watch.hpp"

Watch::Watch() {
    t0 = std::chrono::steady_clock::now();
}

float Watch::getTime() {
    auto t1 = std::chrono::steady_clock::now();
 
    auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0);

    float t = 0.001f * ms_int.count();
    
    return t;
}

void Watch::resetTime() {
    t0 = std::chrono::steady_clock::now();
} 

CountdownTimer::CountdownTimer() : Watch() {
    active = false;
    duration = 0.0f;
}

void CountdownTimer::windGentle(float duration_) {
    if (!active) {
        duration = duration_;
        resetTime();
        active = true;
    }
}

// will deactivate if returns true
bool CountdownTimer::checkDone() {
    if (active && getTime() >= duration) {
        active = false;
        return true;
    } else {
        return false;
    }
}

void CountdownTimer::cancelCountdown() {
    active = false;
}

bool CountdownTimer::isCountingDown() {
    return active;
}
