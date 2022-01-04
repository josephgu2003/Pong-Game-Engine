//
//  HealthMeter.hpp
//  Pong
//
//  Created by Joseph Gu on 9/16/21.
//

#ifndef HealthMeter_hpp
#define HealthMeter_hpp

#include <stdio.h>
#include "uiPiece.hpp"
#include "Observer.hpp" 
 
class HealthMeter : public uiPiece, public Observer {
public:
    HealthMeter(float xpos, float ypos);
    virtual void notify(const Subject& s, GameEvent ge) override;
};

#endif /* HealthMeter_hpp */
 
