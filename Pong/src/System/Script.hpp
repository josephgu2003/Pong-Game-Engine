//
//  ScriptEvent.hpp
//  Pong
//
//  Created by Joseph Gu on 7/2/21.
//

#ifndef Script_hpp
#define Script_hpp

#include <stdio.h>

class Script {
protected:
    int step;
public:
    virtual void tick() = 0;
    virtual void init() = 0;
    virtual void inform() = 0;
};

#endif /* ScriptEvent_hpp */
