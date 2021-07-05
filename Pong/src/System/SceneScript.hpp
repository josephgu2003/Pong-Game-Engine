//
//  SceneScript.hpp
//  Pong
//
//  Created by Joseph Gu on 7/5/21.
//

#ifndef SceneScript_hpp
#define SceneScript_hpp

#include <stdio.h>
#include "Script.hpp"

class Game;

class SceneScript : public Script {
    Game* game;
public:
    void setGame(Game* game);
};

#endif /* SceneScript_hpp */
