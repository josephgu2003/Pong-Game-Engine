//
//  JsonManager.cpp
//  Pong
//
//  Created by Joseph Gu on 9/7/21.
//

#include "JsonManager.hpp"
#include "Game.hpp"
#include "json.hpp"
#include <fstream>
#include <memory>
#include "NameComponent.hpp"
#include <string>
#include "LifeComponent.hpp"
 
void JsonManager::loadGame(Game* game) {
    nlohmann::json saveFile;
    std::ifstream i(SAVE_PATH);
    i >> saveFile;
    
    for (auto i = saveFile.begin(); i != saveFile.end(); i++) {
        if ((*i)["Type"] == "Actor") {
            std::shared_ptr<Actor> actor = std::make_shared<Actor>();
            int actorEnum = (*i)["ActorEnum"];
         //   int numberableID = (*i)["NumberableID"];
            int worldID = (*i)["WorldID"];
            std::string common = (*i)["CommonName"];
            std::string id = (*i)["IDName"];
       
            glm::vec3 pos;
            pos.x =    (*i)["Position"][0];
            pos.y =   (*i)["Position"][1];
            pos.z =    (*i)["Position"][2];
            
            actor->init(actorEnum);
            NameComponent* nc = actor->getComponent<NameComponent>();
            if (nc) {
                actor->getComponent<NameComponent>()->init(common, id);
            }
            LifeComponent* lc = actor->getComponent<LifeComponent>();
            if (nc) {
                float clife, chunger,  cstamina, cenergy = 0;
                float mlife,mhunger,mstamina,  menergy = 1;
                     
                if(i->find("Life") != i->end()) {
                    clife = (*i)["Life"][0] ;
                    mlife = (*i)["Life"][1] ;
                }
                if(i->find("Hunger") != i->end()) {
                    chunger = (*i)["Hunger"][0] ;
                    mhunger = (*i)["Hunger"][1] ;
                }
                if(i->find("Stamina") != i->end()) {
                    cstamina = (*i)["Stamina"][0] ;
                    mstamina = (*i)["Stamina"][1] ;
                }
                if(i->find("Energy") != i->end()) {
                    cenergy = (*i)["Energy"][0] ;
                    menergy = (*i)["Energy"][1] ;
                }
               lc->init(clife, mlife, chunger, mhunger, cstamina, mstamina, cenergy, menergy);
            }
           // actor->setID(numberableID);
            actor->setPos(pos);
            actor->setWorld(&game->getWorld(worldID));
           // game->setNumberable(actor.get(), numberableID);
            game->getWorld(worldID).insertActor(actor);
            
            if ((*i)["SubType"] == "PlayerHero") {
                game->setPlayerHero(actor, worldID);
            }
        }
    }
}

void JsonManager::saveGame(Game* game) {
    nlohmann::json saveFile;
    std::ifstream i(SAVE_PATH);
    i >> saveFile;
    
    for (auto i = saveFile.begin(); i != saveFile.end(); i++) {
        if ((*i)["Type"] == "Actor") {
            std::string id = (*i)["IDName"];
            Actor* actor = game->getWorld(0).getActorNamed(id).get();
            
            if (actor->dummy) {
                actor = game->getWorld(1).getActorNamed(id).get();
            }
            
            if (actor->dummy) {
                continue;
            }
            
            glm::vec3 pos = actor->getPos();
            (*i)["Position"][0] = pos.x;
            (*i)["Position"][1] = pos.y;
            (*i)["Position"][2] = pos.z;
            LifeComponent* lc = actor->getComponent<LifeComponent>();
            if (lc) {
               //     if(i->find("Life") != i->end()) {
                    (*i)["Life"][0] = lc->getStat(STAT_LIFE).value;
                    (*i)["Life"][1] = lc->getStat(STAT_LIFE).max;
                   // }
                (*i)["Hunger"][0] = lc->getStat(STAT_HUNGER).value;
                (*i)["Hunger"][1] = lc->getStat(STAT_HUNGER).max;
                
                (*i)["Stamina"][0] = lc->getStat(STAT_STAMINA).value;
                (*i)["Stamina"][1] = lc->getStat(STAT_STAMINA).max;
                
                (*i)["Energy"][0] = lc->getStat(STAT_ENERGY).value;
                (*i)["Energy"][1] = lc->getStat(STAT_ENERGY).max;
            }
        }
    }
    std::ofstream o(SAVE_PATH);
    o << std::setw(4) << saveFile << std::endl;
}  
