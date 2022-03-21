//
//  Behaviour.hpp
//  Pong
//
//  Created by Joseph Gu on 12/21/21.
//

#ifndef Behaviour_hpp
#define Behaviour_hpp

#include <stdio.h> 

class Behaviour {
protected:
    bool running;
public:
    Behaviour();
    virtual void start();
    virtual void tick();
    bool isRunning();
};

#endif /* Behaviour_hpp */
