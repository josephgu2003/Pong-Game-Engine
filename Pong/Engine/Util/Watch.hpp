//
//  Watch.hpp
//  Pong
//
//  Created by Joseph Gu on 10/7/21.
//

#ifndef Watch_hpp
#define Watch_hpp

#include <stdio.h>
#include <chrono>

// first tick might be messed up
// doesnt make everything in the game tick have same time, we need a time thats updated per tick but not between ticks in that case

// counts time as it progresses
class Watch {
private:
    std::chrono::time_point<std::chrono::steady_clock> t0;
public:
    Watch();
    float getTime();
    void resetTime();
};

// counts down to a point and then deactivates
class CountdownTimer : public Watch {
    float duration;
    bool active;
public:
    CountdownTimer();
    
    // winds the clock
    void windGentle(float duration);
    
    // will deactivate if returns true
    bool checkDone();
};


#endif /* Watch_hpp */
