//
//  TheLake.hpp
//  Pong
//
//  Created by Joseph Gu on 2/12/22.
//

#ifndef TheLake_hpp
#define TheLake_hpp

#include <stdio.h>
#include "Script.hpp"
   
class TheLake : public Script {
public:
    TheLake(World* world, bool completed);
    virtual void act() override;
}; 

#endif /* TheLake_hpp */ 
