//
//  MyGame.hpp
//  Pong
//
//  Created by Joseph Gu on 10/30/21.
//

#ifndef MyGame_hpp
#define MyGame_hpp

#include <stdio.h>
#include "Game.hpp"
#include "MyActorFactory.hpp"
#include "ParticleFactory.hpp"
#include "PropFactory.hpp"
#include "ScriptFactory.hpp"
 
class MyGame : public Game {
    MyActorFactory actorFactory;
    ParticleFactory particleFactory;
    PropFactory propFactory;
    ScriptFactory scriptFactory;
    LevelBuilder myLvlBuilder;
    void setupLvlBuilder();
public:
    MyGame();
    virtual void load() override;
    
};
#endif /* MyGame_hpp */
