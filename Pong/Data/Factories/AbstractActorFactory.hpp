//
//  AbstractActoryFactory.hpp
//  Pong
//
//  Created by Joseph Gu on 10/31/21.
//

#ifndef AbstractActorFactory_hpp
#define AbstractActorFactory_hpp

#include <stdio.h>
#include <memory.h>
#include "Actor.hpp"

class AbstractActorFactory {
private:
public: 
    virtual std::shared_ptr<Actor> makeActor(int i) = 0;
};
 
#endif /* AbstractActorFactory_hpp */
