//
//  InTheWilderness.hpp
//  Pong
//
//  Created by Joseph Gu on 3/16/22.
//

#ifndef InTheWilderness_hpp
#define InTheWilderness_hpp

#include <stdio.h>
#include "MyScript.hpp"
#include <set>
#include <queue>
#include "DirectionalLight.hpp"

class uiLayout;

static const DirectionalLight midday(glm::vec3(0.13,0.1,0.1),glm::vec3(1.4 * 0.99, 1.4 * 0.98, 1.4 * 0.82),glm::vec3(1.3,1.3,1.4),glm::vec3(-1,-1,0));

static const DirectionalLight sunset(glm::vec3(0.07,0.04,0.04),glm::vec3(0.8, 0.38, 0.33), glm::vec3(1.3,0.7,0.6),glm::vec3(-1,-0.3,-1));

static const DirectionalLight earlyNight(glm::vec3(0.05,0.04,0.05),glm::vec3(0.35, 0.28, 0.33), glm::vec3(0.8,0.6,0.8),glm::vec3(-1,-1,0));

static const DirectionalLight midNight(glm::vec3(0.02,0.01,0.03),glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0,0.0,0.0),glm::vec3(-1,-1,0));

class InTheWilderness : public MyScript {
private:
    std::set<std::string> paintedSubjects;
    CountdownTimer floroPauseSpeak;
    glm::vec3 flowerPos;
    glm::vec3 treePos;
    bool encounterFlower;
    bool encounterTree;
    
    std::queue<std::string> floroNextLines;
    
    std::queue<std::string> floroRandomQuotes;
    CountdownTimer randomQuoteInterval;
public:
    InTheWilderness(World* world, bool completed, InputHandler* ih, uiLayout* ui);
    void act() override;
    
};
#endif /* InTheWilderness_hpp */
