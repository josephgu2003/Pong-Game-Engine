//
//  SaveGame.hpp
//  Pong
//
//  Created by Joseph Gu on 12/30/21.
//

#ifndef SaveGame_hpp
#define SaveGame_hpp

#define GAME_SAVE_PATH "Resources/GameData/GameSaveFile.json"

#include <stdio.h>
#include "json.hpp"
#include "GameLevel.hpp"

class SaveObject {
    protected:
    nlohmann::json jsonStash;
public:
    virtual void load(GameLevel* g, const nlohmann::json& json) {}
    virtual nlohmann::json save() {return nlohmann::json();}
};

class SaveSystem {
public:
    virtual void loadGameLevel(GameLevel* g) {}
    virtual void saveGameLevel(GameLevel* g) {}
};

class SaveGame : public SaveSystem { 
private:
    std::vector<std::shared_ptr<SaveObject>> saveObjects;
    virtual SaveObject* handleObjectOfType(std::string type) = 0;
public:
    void loadGameLevel(GameLevel* g) override;
    
    void saveGameLevel(GameLevel* g) override;
};

#endif /* SaveGame_hpp */
