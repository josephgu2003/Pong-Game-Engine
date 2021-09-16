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


class Game;
 
namespace JsonManager {
    void loadGame(Game* game);
    void saveGame(Game* game);
};


#endif /* JsonManager_hpp */
