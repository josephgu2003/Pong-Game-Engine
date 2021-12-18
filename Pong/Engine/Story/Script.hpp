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
    float radius;
    std::unordered_map<std::string, std::weak_ptr<Actor>> actors;
protected:
    Watch stopWatch;
    int step;
    World* world = nullptr;
    Actor* getActorNamed(std::string name);
    void incStep(bool resetTime);
    void waitFor(float duration);
    bool isWaiting();
    void newActor(std::string name, const std::shared_ptr<Actor>& actor);
public:
    Script(World* world, std::vector<std::string> crew, float radius);
    void tick();
    virtual void act() = 0; 
};

#endif /* ScriptEvent_hpp */
