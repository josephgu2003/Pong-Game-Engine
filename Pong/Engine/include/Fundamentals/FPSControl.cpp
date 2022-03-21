//
//  FPSControl.cpp
//  Pong
//
//  Created by Joseph Gu on 11/28/21.
//

#include "FPSControl.hpp"
#include <thread>
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

FPSControl::FPSControl() {
    fps = 30;
    watch.resetTime();
    timerForTick.resetTime();
}

bool FPSControl::regulateFPS() {
    float ratio = (float) watch.getTime()/(fps/1000.0f);
    if (floor(ratio) == intervalTimer) {
        intervalTimer = floor(ratio);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        return false;
    }
    else {
        intervalTimer = floor(ratio);
    }
     
    float elapsedT = timerForTick.getTime();
    glfwSetTime(elapsedT);
    timerForTick.resetTime();
    fpsTimer += elapsedT;
    
    draws = draws + 1.0;
    if (fpsTimer > 2.0) {
        float fps = draws/2.0;
     //   printf("Fps is %f\n", fps);
        fpsTimer = 0.0;
        draws = 0.0;
    }
    return true;
}

void FPSControl::setFPS(int fps_) {
    fps = fps_;
}
