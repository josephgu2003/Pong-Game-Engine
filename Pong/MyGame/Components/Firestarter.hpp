//
//  Firestarter.hpp
//  Pong
//
//  Created by Joseph Gu on 3/20/22.
//

#ifndef Firestarter_hpp
#define Firestarter_hpp

#include <stdio.h>
#include "Component.hpp"
#include "Observer.hpp"
#include <string>
#include "Watch.hpp"

class Actor;
class World;

class FireStarter : public Component, public Observer {
    Actor& actorRef;

    CountdownTimer lightingTime;

public:
    FireStarter(Componentable& compUser_, Actor& actor);
    
    void tick() override;
    
    void notify(const Subject &subject, GameEvent ge) override;
    
};


#endif /* Firestarter_hpp */
