//
//  Speech.hpp
//  Pong
//
//  Created by Joseph Gu on 6/7/21.
//

#ifndef Speech_hpp
#define Speech_hpp

#include <stdio.h>
#include "Behaviour.hpp"
#include "Watch.hpp"
#include <vector>
#include <string>

class Actor;

class Speech: public Behaviour {
    float duration;
    Watch watch;
    int step;
    Actor* actor = NULL;
    std::vector<std::string> lines;
    std::vector<float> durations;
    float elapsetime;
    inline float fetchDuration(int i) {
        if (i < durations.size()) {
            return durations.at(i);
        } else {
            return 0.1f;
        }
    }
     
public:
    Speech(Actor* actor_, std::vector<std::string>& strings, std::vector<float>& durations);
    virtual void start() override;
    virtual void tick() override;
};
    
#endif /* Speech_hpp */


