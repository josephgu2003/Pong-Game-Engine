//
//  LevelSerializer.hpp
//  Pong
//
//  Created by Joseph Gu on 12/30/21.
//

#ifndef LevelSerializer_hpp
#define LevelSerializer_hpp

#define GAMELVLS_PATH "Resources/GameData/GameLevels.json"

#include <stdio.h>
#include "GameLevel.hpp"
#include "json.hpp"

class World;

class LevelSerializer {
private:
    virtual void convertPrimitive(World* g, const std::string& type, int primitiveEnum, const glm::vec3& pos) = 0;
public:
    void loadLevelWorlds(GameLevel* g);
};

#endif /* LevelSerializer_hpp */
