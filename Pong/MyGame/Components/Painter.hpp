//
//  Painter.hpp
//  Pong
//
//  Created by Joseph Gu on 3/17/22.
//

#ifndef Painter_hpp
#define Painter_hpp

#include <stdio.h>
#include "Component.hpp"
#include "Watch.hpp"
#include "Observer.hpp"
#include <string> 
class Actor;
class World;

class Painter : public Component, public Observer {
    Actor& actorRef;
    Componentable* targetedSubject;
    Watch paintTime;
    bool paintInProgress;
    std::string recentPainting;
public:
    Painter(Componentable& compUser_, Actor& actor);
    
    void tick() override;
    
    void notify(const Subject &subject, GameEvent ge) override;
    
    std::string getMostRecentPainting() const;
    
};


#endif /* Painter_hpp */
