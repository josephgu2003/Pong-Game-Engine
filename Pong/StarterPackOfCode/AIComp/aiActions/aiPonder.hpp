//
//  aiPonder.hpp
//  Pong
//
//  Created by Joseph Gu on 10/16/21.
//

#ifndef aiPonder_hpp
#define aiPonder_hpp

#include <stdio.h>
#include "AIAction.hpp"
#include "Watch.hpp"

class aiPonder : public aiAction {
private:
    Watch myWatch;
public:
    aiPonder(float i);
    virtual void tick(Actor* actor) override;
};

#endif /* aiPonder_hpp */
