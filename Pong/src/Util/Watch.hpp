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
class Watch {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> t0;
public:
    Watch();
    float getTime();
    void resetTime();
};


#endif /* Watch_hpp */
