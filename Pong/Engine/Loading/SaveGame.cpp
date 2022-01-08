//
//  SaveGame.cpp
//  Pong
//
//  Created by Joseph Gu on 12/30/21.
//

#include "SaveGame.hpp"
#include <fstream>

void SaveGame::loadGameLevel(GameLevel *g){
    nlohmann::json saveFile;
    std::ifstream inStream(GAME_SAVE_PATH);
    inStream >> saveFile;
     
    nlohmann::json lvlSave = saveFile.find(g->getName()).value();
     
    for (auto i = lvlSave.begin(); i != lvlSave.end(); i++) {
        std::unique_ptr<SaveObject> newObj;
        newObj.reset(handleObjectOfType((*i)["Type"]));
        newObj->load(g,(*i));
        saveObjects.push_back(std::move(newObj));
    }
}

void SaveGame::saveGameLevel(GameLevel *g){
    nlohmann::json saveFile;
    for (auto i = saveObjects.begin(); i != saveObjects.end(); i++) {
        saveFile.push_back((*i)->save());
    }
    
    nlohmann::json save;
    std::ifstream inStream(GAME_SAVE_PATH);
    inStream >> save;
     
    save[g->getName().c_str()] = saveFile;
    
    std::ofstream o(GAME_SAVE_PATH);
    o << std::setw(4) << save << std::endl;
}
