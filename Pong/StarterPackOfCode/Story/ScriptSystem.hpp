//
//  ScriptSystem.hpp
//  Pong
//
//  Created by Joseph Gu on 12/25/21.
//

#ifndef ScriptSystem_hpp
#define ScriptSystem_hpp

#include <stdio.h>
#include "WorldSubSystem.hpp" 
#include "Script.hpp"

class ScriptSystem : public WorldSubSystem {
private: 
    std::vector<std::shared_ptr<Script>> allScripts;
public:
    using WorldSubSystem::WorldSubSystem;
    void insertScript(std::shared_ptr<Script>& script);
    void tick() override;
    bool isScriptComplete(const std::string& name); 
};
 
// world owns scripts
// this owns scripts


#endif /* ScriptSystem_hpp */
