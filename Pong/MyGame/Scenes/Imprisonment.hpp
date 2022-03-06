//
//  Imprisonment.hpp
//  Pong
//
//  Created by Joseph Gu on 3/4/22.
//

#ifndef Imprisonment_hpp
#define Imprisonment_hpp

#include <stdio.h>
#include "Script.hpp"
#include "Prop.hpp"

class Imprisonment : public Script {
private:
    std::weak_ptr<Prop> imprisonment;
    Watch watch;
    float wallCooldown;
public:
    Imprisonment(World* world, bool completed);
    virtual void act() override;
};

#endif /* Imprisonment_hpp */
