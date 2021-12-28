//
//  MyActorFactory.hpp
//  Pong
//
//  Created by Joseph Gu on 10/30/21.
//

#ifndef MyActorFactory_hpp
#define MyActorFactory_hpp

#include <stdio.h>
#include "Actor.hpp"
#include <memory>
#include "AbstractActorFactory.hpp"

enum ActorEnum {
    ACTOR_HOODY,
    ACTOR_EVIL_HOODY, 
    ACTOR_BIRD,
    ACTOR_VAMP,
    ACTOR_SCARF_CHAR,
    ACTOR_FISH,
    ACTOR_KNIGHT
};
class MyActorFactory : public AbstractActorFactory {
private:
public: 
    MyActorFactory();
    virtual std::shared_ptr<Actor> makeActor(int i) override;
};

#endif /* MyActorFactory_hpp */
