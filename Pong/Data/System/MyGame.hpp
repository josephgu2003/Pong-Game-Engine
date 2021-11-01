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
 
class MyGame : public Game {
    MyActorFactory actorFactory;
    ParticleSystem* mist = nullptr;
public:
    MyGame();
    virtual void load() override;
    
};
#endif /* MyGame_hpp */
