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

class InputHandler;

class ScriptFactory {
private:
    std::weak_ptr<uiLayout> ui;
    InputHandler* ih;
public:
    ScriptFactory(); 
    void setUI(const std::shared_ptr<uiLayout> ui_);
    void setInputHandler(InputHandler* ih);
    std::shared_ptr<Script> makeScript(std::string scriptname, World* world, bool completed);
};

#endif /* ScriptFactory_hpp */
