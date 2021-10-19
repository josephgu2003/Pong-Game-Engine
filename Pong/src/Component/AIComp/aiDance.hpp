//
//  aiDance.hpp
//  Pong
//
//  Created by Joseph Gu on 10/9/21.
//

#ifndef aiDance_hpp
#define aiDance_hpp

#include <stdio.h>
#include "AIAction.hpp"
#include "Watch.hpp"

class aiDance : public aiAction {
private:
    Watch myWatch; 
public:
    aiDance(float i);
    virtual void tick(Actor* actor) override;
};
#endif /* aiDance_hpp */
