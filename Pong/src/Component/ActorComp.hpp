//
//  ActorComp.hpp
//  Pong
//
//  Created by Joseph Gu on 10/20/21.
//

#ifndef ActorComp_hpp
#define ActorComp_hpp

#include <stdio.h>
#include "Component.hpp"

class Actor;

class ActorComp : public Component<Actor> {
protected:
public:
    ActorComp(Actor& actor);
};

#endif /* ActorComp_hpp */
