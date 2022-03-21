//
//  FPSControl.hpp
//  Pong
//
//  Created by Joseph Gu on 11/28/21.
//

#ifndef FPSControl_hpp
#define FPSControl_hpp

#include <stdio.h>
#include "Watch.hpp"

class FPSControl {
    double fpsTimer;
    int intervalTimer = 0;
    float draws;
    Watch timerForTick;
    Watch watch;
    int fps;
public:
    FPSControl();
    bool regulateFPS();
    void setFPS(int fps);
};

#endif /* FPSControl_hpp */
