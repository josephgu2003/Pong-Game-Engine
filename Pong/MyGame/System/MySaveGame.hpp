//
//  MySaveGame.hpp
//  Pong
//
//  Created by Joseph Gu on 12/31/21.
//

#ifndef MySaveGame_hpp
#define MySaveGame_hpp

#include <stdio.h>
#include "SaveGame.hpp"

class uiLayout;

class MySaveGame : public SaveGame {
    SaveObject* handleObjectOfType(std::string type) override;
protected:

public:
    inline static void loadPos(glm::vec3& pos, const nlohmann::json& posdata) {
        pos.x =    (posdata)["Position"][0];
        pos.z =    (posdata)["Position"][2];
        pos.y =   posdata["Position"][1];
    }
    
    inline static std::vector<float> convertPos(glm::vec3 pos_) {
        std::vector<float> pos(3);
        pos[0] = pos_.x;
        pos[1] = pos_.y;
        pos[2] = pos_.z;
        return pos;
    }
    MySaveGame(std::shared_ptr<uiLayout>& ui);
};

#include "Actor.hpp"
#include "MyActorFactory.hpp"

class SaveChar : public SaveObject {
protected:
    std::weak_ptr<Actor> actorref;
    MyActorFactory af;
public:
    virtual void load(GameLevel* g, const nlohmann::json& i) override;

    virtual nlohmann::json save() override;
};
class SavePlayer : public SaveChar {
public:
    void load(GameLevel* g, const nlohmann::json& json) override;
};


#include "ScriptFactory.hpp"
#include "Script.hpp"
class SaveScript : public SaveObject {
    static ScriptFactory sf;
    std::weak_ptr<Script> scriptref;
    int worldID;
    std::string factoryEnum;
    glm::vec3 pos_;
public:
    static void setUI(std::shared_ptr<uiLayout>& ui);
    void load(GameLevel* g, const nlohmann::json& json) override;
    nlohmann::json save() override;
};


#endif /* MySaveGame_hpp */
