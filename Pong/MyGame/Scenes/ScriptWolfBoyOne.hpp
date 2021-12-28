//
//  ScriptWolfBoyOne.hpp
//  Pong
//
//  Created by Joseph Gu on 12/25/21.
//

#ifndef ScriptWolfBoyOne_hpp
#define ScriptWolfBoyOne_hpp

#include <stdio.h>
#include "Script.hpp"

class ScriptWolfBoyOne : public Script{
public:
    ScriptWolfBoyOne(World* world, bool completed);
    virtual void act() override;
};

#endif /* ScriptWolfBoyOne_hpp */
