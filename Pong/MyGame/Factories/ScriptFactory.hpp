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
#include "uiLayout.hpp"

class ScriptFactory {
private:
    std::weak_ptr<uiLayout> ui;
public:
    void setUI(const std::shared_ptr<uiLayout> ui_);
    std::shared_ptr<Script> makeScript(std::string scriptname, World* world, bool completed);
};

#endif /* ScriptFactory_hpp */
