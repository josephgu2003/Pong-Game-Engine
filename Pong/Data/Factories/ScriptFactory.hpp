//
//  ScriptFactory.hpp
//  Pong
//
//  Created by Joseph Gu on 11/13/21.
//

#ifndef ScriptFactory_hpp
#define ScriptFactory_hpp

#include <stdio.h>
#include "Script.hpp"

class ScriptFactory {
public:
    std::shared_ptr<Script> makeScript(int scriptenum, World* world);
};

#endif /* ScriptFactory_hpp */
