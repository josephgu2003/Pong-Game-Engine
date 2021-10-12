//
//  JsonManager.hpp
//  Pong
//
//  Created by Joseph Gu on 9/7/21.
//

#ifndef JsonManager_hpp
#define JsonManager_hpp

#define SAVE_PATH "Data/SaveFile.json"

#include <stdio.h>
#include <string>
#include <memory>
#include <vector>
#include "Relationship.hpp"

class Game;
 
namespace JsonManager {
    void loadGame(Game* game);
    void saveGame(Game* game);
    bool checkRelationshipLoaded(std::string aname, std::string bname, std::string type, const std::vector<std::shared_ptr<Relationship>>&  loaded);
};


#endif /* JsonManager_hpp */
