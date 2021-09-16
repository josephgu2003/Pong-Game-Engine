//
//  ScriptOne.hpp
//  Pong
//
//  Created by Joseph Gu on 7/9/21.
//

#ifndef ScriptOne_hpp
#define ScriptOne_hpp

#include <stdio.h>
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include "Script.hpp"

#include "World.hpp"
#include "Actor.hpp"
#include <glm/glm.hpp>
#include <memory>

class ScriptOne : public Script {
   Actor* pHero = NULL;
    Actor* ball = NULL;
    float timer = 0;
    
    enum State {
        STATE_AGGRESSIVE,
        STATE_PASSIVE
    } state = STATE_PASSIVE;
    
    enum BallDir {
        STATE_TRACK,
        STATE_OPPOSITE
    } ballDir = STATE_TRACK;
    glm::vec3 horizonDir;
    
    std::shared_ptr<Ability> currentAbility;
    std::shared_ptr<Ability> currentAbility2;
public:
    virtual void tick() override;
    virtual void init(Game* game) override;
};
#endif /* ScriptOne_hpp */
