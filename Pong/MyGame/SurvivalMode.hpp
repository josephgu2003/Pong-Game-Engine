//
//  SurvivalMode.hpp
//  Pong
//
//  Created by Joseph Gu on 1/25/22.
//

#ifndef SurvivalMode_hpp
#define SurvivalMode_hpp

#include <stdio.h>
#include "Component.hpp"
#include "Watch.hpp"
#include "Dice.hpp"
#include "MyActorFactory.hpp"
#include <queue>

class World;

class SurvivalMode : public Component {
private:
    int numberHostiles;
    Watch watch;
    Dice dice;
    MyActorFactory maf;
    float spawnRadius;
    std::queue<std::weak_ptr<Actor>> sentHunters;
    
    bool checkHuntersFree();
    
    float healthDivisor = 1.0f;
    
    glm::vec3 randomPos(const glm::vec3& origin);
public:
    SurvivalMode(World& w, float radius);
    void tick() override;
};

class Hunter : public Component {
private:
    Watch watch;
public:
    using Component::Component;
    void tick() override;
};

#endif /* SurvivalMode_hpp */
