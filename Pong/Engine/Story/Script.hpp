//
//  ScriptEvent.hpp
//  Pong
//
//  Created by Joseph Gu on 7/2/21.
//

#ifndef Script_hpp
#define Script_hpp

#include <stdio.h>
#include <vector>
#include <unordered_map>
#include "Positionable.hpp"
#include "Actor.hpp"
#include "Watch.hpp"
class World;

class Script : public Positionable {
private:
    bool checkAllHere();
    std::shared_ptr<Actor> dummy;
    float lastTime;
protected:
    Watch stopWatch;
    int step;
    World* world = nullptr;
    std::unordered_map<std::string, std::shared_ptr<Actor>> actors;
    void incStep(bool resetTime);
    void waitFor(float duration);
    bool isWaiting();
public:
    Script(World* world, std::vector<std::string> crew);
    void tick();
    virtual void act() = 0; 
};

#endif /* ScriptEvent_hpp */
