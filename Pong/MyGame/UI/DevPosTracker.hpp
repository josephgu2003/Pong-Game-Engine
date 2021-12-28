//
//  DevPosTracker.hpp
//  Pong
//
//  Created by Joseph Gu on 12/11/21.
//

#ifndef DevPosTracker_hpp
#define DevPosTracker_hpp

#include <stdio.h>
#include "uiText.hpp"
#include "Observer.hpp"

class DevPosTracker : public uiText, public Observer {
private:
public:
    DevPosTracker(float xpos, float ypos, float xscale, float yscale);
    virtual void notify(const Subject& s, GameEvent ge) override;
};


#endif /* DevPosTracker_hpp */
