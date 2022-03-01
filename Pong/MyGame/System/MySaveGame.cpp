//
//  MySaveGame.cpp
//  Pong
//
//  Created by Joseph Gu on 12/31/21.
//

#include "MySaveGame.hpp"
#include "NameComponent.hpp" 
#include "CharacterComponent.hpp"
#include "LifeComponent.hpp"
#include "ScriptSystem.hpp"
#include "InventoryComponent.hpp"

ScriptFactory SaveScript::sf;

void SaveChar::load(GameLevel* g, const nlohmann::json& i) {
        auto handleName = [] (std::shared_ptr<Actor>& a, const nlohmann::json& i) {
            if (NameComponent* nc = a->getComponent<NameComponent>()) {
                std::string name = i.find("Name").value();
                assert(name != "");
                a->getComponent<NameComponent>()->init(name);
            }
        }; 
             
    auto handleInvComp = [] (std::shared_ptr<Actor>& a, const nlohmann::json& i) {
        if (InventoryComponent* ic = a->getComponent<InventoryComponent>()) {
            auto itemblock = i.find("Items");
            if (itemblock != i.end()) {
                auto items = itemblock.value();
                for (int i = 0; i < items.size(); i++) {
                    ic->insertItem(items.at(i));
                }
            }
        }
    };
    
            auto handleCharComp = [] (std::shared_ptr<Actor>& a, const nlohmann::json& i) {
                if (CharacterComponent* charc = a->getComponent<CharacterComponent>()) {
                    std::string id = i["Name"];
                    if(i.find("Relationships") != i.end()) {
                        auto field = i["Relationships"];
                        for (int i = 0; i < field.size(); i++) {
                            auto entry = field.at(i);
                            std::shared_ptr<Relationship> rs = std::make_shared<Relationship>(  entry["Name1"],entry["Name2"], entry["RelationshipIntensity"], entry["RelationshipType"],entry["SecretNickname1"],  entry["SecretNickname2"]);
                                charc->newRelationship(rs);
                        }
                    }
                }
            };
            
            auto handleLifeComp = [] (std::shared_ptr<Actor>& a, const nlohmann::json& i) {
                if (LifeComponent* lc = a->getComponent<LifeComponent>()) {
                    float clife, chunger,  cstamina, cenergy = 0.0f;
                    float mlife,mhunger,mstamina,  menergy = 1.0f;
                        
                    auto handleStat = [&] (std::string s, float& val, float& max) {
                        auto stat = i.find(s);
                        if(stat != i.end()) {
                            val = stat.value()[0];
                            max = stat.value()[1];
                        }
                    };
                    
                    handleStat("Life", clife, mlife);
                    handleStat("Hunger", chunger, mhunger);
                    handleStat("Stamina", cstamina, mstamina);
                    handleStat("Energy", cenergy, menergy);

                   lc->init(clife, mlife, chunger, mhunger, cstamina, mstamina, cenergy, menergy);
                }
            };
            
            int FactoryEnum = i["FactoryEnum"];
            std::shared_ptr<Actor> actor = af.makeActor(FactoryEnum);
            int worldID = i["WorldID"];
        
            glm::vec3 pos;
            MySaveGame::loadPos(pos, i);
            
            handleName(actor, i);
            handleCharComp(actor, i);
            handleLifeComp(actor, i);
            handleInvComp(actor, i);
     
            actor->setPos(pos); 
              
            World& world = g->getWorld(worldID);
            actor->setWorld(&world);

            world.insert<Actor>(actor);
            actorref = actor;
            jsonStash = i;
        }
nlohmann::json SaveChar::save() {
            if (!jsonStash.empty()) {
                std::string id = jsonStash["Name"]; 
         
                if (auto a = actorref.lock()) {
                    auto vec = MySaveGame::convertPos(a->getPos());
                    jsonStash.emplace("Position", vec);
                    if (LifeComponent* lc = a->getComponent<LifeComponent>()) {
                        
                        auto handleStat = [&] (std::string name, StatType s) {
                            jsonStash[name][0] = lc->getStat(s).value;
                            jsonStash[name][1] = lc->getStat(s).max;
                        };
     
                        handleStat("Life", STAT_LIFE);
                        handleStat("Hunger", STAT_HUNGER);
                        handleStat("Energy", STAT_ENERGY);
                        handleStat("Stamina", STAT_STAMINA);
                    }
                return jsonStash;
                }
            }
            nlohmann::json saveFile;
            return saveFile;
    }

void SavePlayer::load(GameLevel* g, const nlohmann::json& json) {
            SaveChar::load(g, json);
            World& world = g->getWorld(json["WorldID"]);
            world.markPlayerHero(actorref.lock().get());
}

void SaveScript::setUI(std::shared_ptr<uiLayout>& ui) {
    sf.setUI(ui); 
}
 
void SaveScript::load(GameLevel* g, const nlohmann::json& json) {
            std::string FactoryEnum = json["FactoryEnum"];
            factoryEnum = FactoryEnum;
            World& world = g->getWorld(json["WorldID"]);
            worldID = json["WorldID"];
            bool completed = json["Completed"];
        
            std::shared_ptr<Script> script = sf.makeScript(FactoryEnum, &world, completed);
            glm::vec3 pos;
            MySaveGame::loadPos(pos, json);
                 
            script->setPos(pos); 
            
            scriptref = script;
            pos_ = pos;
            
            world.getComponent<ScriptSystem>()->insertScript(script);
        } 
nlohmann::json SaveScript::save() {
            nlohmann::json saveFile;
            saveFile.emplace("Type","Script");
            saveFile.emplace("WorldID", worldID);
            saveFile.emplace("FactoryEnum", factoryEnum);
            saveFile.emplace("Position", MySaveGame::convertPos(pos_));
            if (auto s = scriptref.lock()) {
                saveFile.emplace("Completed",s->isComplete());
            }
            return saveFile;
}
    
SaveObject* MySaveGame::handleObjectOfType(std::string type) {
        if (type == "Char") return new SaveChar();
        if (type == "Player") return new SavePlayer();
        if (type == "Script") return new SaveScript();
        return new SaveObject();
    }
    
MySaveGame::MySaveGame(std::shared_ptr<uiLayout>& ui) {
        SaveScript::setUI(ui);
    }
 
 
