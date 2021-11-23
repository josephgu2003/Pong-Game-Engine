//
//  JsonManager.cpp
//  Pong
//
//  Created by Joseph Gu on 9/7/21.
//

#include "JsonManager.hpp"
#include "Game.hpp"
#include <fstream>
#include "NameComponent.hpp" 
#include "CharacterComponent.hpp"
#include "LifeComponent.hpp" 
#include "Dialogue.hpp"
#include "AbstractActorFactory.hpp"
#include "PropFactory.hpp"
#include "ScriptFactory.hpp"

nlohmann::json JsonManager::dialogues;

void JsonManager::loadGame(Game* game, AbstractActorFactory* af, PropFactory* propf, ParticleFactory* pf, ScriptFactory* sf) {
    nlohmann::json saveFile;
    std::ifstream inStream(SAVE_PATH);
    inStream >> saveFile;
      
    for (auto i = saveFile.begin(); i != saveFile.end(); i++) {
        std::vector<std::shared_ptr<Relationship>> relationships;
        // possible errors for relationships: corrupted in one actor's json, how to fix?
        // or the FactoryEnum doesn't generate a CharacterComponent
        if ((*i)["Type"] == "Script") {
            int FactoryEnum = (*i)["FactoryEnum"];
            int worldID = (*i)["WorldID"]; 
            World& world = game->getWorld(worldID);
        
            std::shared_ptr<Script> script = sf->makeScript(FactoryEnum, &world);
            glm::vec3 pos;
            pos.x =    (*i)["Position"][0];
            pos.z =    (*i)["Position"][2];
            pos.y =   world.getHeightAt(glm::vec2(pos.x, pos.z));
            
            script->setPos(pos); 
            game->getWorld(worldID).insert<Script>(script);
        }  
        
        if ((*i)["Type"] == "Prop") {
            int FactoryEnum = (*i)["FactoryEnum"];
            std::shared_ptr<Prop> actor = propf->makeProp(FactoryEnum); // lmao still "actor"??
            int worldID = (*i)["WorldID"];
            World& world = game->getWorld(worldID);
        
            glm::vec3 pos;
            pos.x =    (*i)["Position"][0];
            pos.z =    (*i)["Position"][2];
            pos.y =   world.getHeightAt(glm::vec2(pos.x, pos.z));
            
            actor->setPos(pos);
            actor->setWorld(&game->getWorld(worldID)); 
            game->getWorld(worldID).insert<Prop>(actor); 
        }
        
        if ((*i)["Type"] == "Actor") { 
            int FactoryEnum = (*i)["FactoryEnum"];
            std::shared_ptr<Actor> actor = af->makeActor(FactoryEnum);
            int worldID = (*i)["WorldID"];
        
            glm::vec3 pos;
            pos.x =    (*i)["Position"][0];
            pos.y =   (*i)["Position"][1];
            pos.z =    (*i)["Position"][2];
            
            NameComponent* nc = actor->getComponent<NameComponent>();
            if (nc) {
                std::string name = (*i)["Name"];
                actor->getComponent<NameComponent>()->init(name);
            }
            
            CharacterComponent* charc = actor->getComponent<CharacterComponent>();
            if (charc) {
                std::string id = (*i)["Name"];
                if(i->find("Relationships") != (*i).end()) {
                    auto field = (*i)["Relationships"];
                    for (int i = 0; i < field.size(); i++) {
                        auto entry = field.at(i);
                 
                            std::shared_ptr<Relationship> rs = std::make_shared<Relationship>(entry["Name1"],entry["Name2"], entry["RelationshipIntensity"],entry["RelationshipType"], entry["SecretNickname1"], entry["SecretNickname2"]);
                            charc->newRelationship(rs);
                        
                    } 
                }
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
            actor->setPos(pos);
            actor->setWorld(&game->getWorld(worldID));
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
            std::string id = (*i)["Name"];
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

bool JsonManager::checkRelationshipLoaded(std::string aname, std::string bname, RelationShipType type,  const std::vector<std::shared_ptr<Relationship>>&  loaded) {
    for (int i = 0; i < loaded.size(); i++) {
        if (loaded.at(i)->getName(RS_ONE) == aname) {
            if (loaded.at(i)->getName(RS_TWO) == bname) {
                return true;
            }
        }
    }
    return false;
}


void JsonManager::buildTree(DialogueTree*& tree, int i, int branchID) {
    if (branchID == -1)  {
        tree = nullptr;
        return;
    }
    for (int j = 0; j < dialogues.at(i)["Dialogue"].size(); j++) {
        if (dialogues.at(i)["Dialogue"].at(j)["LineID"] == branchID) { tree->lines.insert(tree->lines.end(),       dialogues.at(i)["Dialogue"].at(j)["Lines"].begin(), dialogues.at(i)["Dialogue"].at(j)["Lines"].end());
        tree->left = new DialogueTree(); //interesting, these 2 lines are necessary or else original tree back in loadDialogue is different pointer than the tree in the first buildtree call
        tree->right = new DialogueTree();
        buildTree((tree->left), i, dialogues.at(i)["Dialogue"].at(j)["LeftID"]);
        buildTree((tree->right), i, dialogues.at(i)["Dialogue"].at(j)["RightID"]);
        return;
        }
    }
}

void JsonManager::loadDialogue(Dialogue* dialogue, int id) {
    if (dialogues.size() == 0) { //check if loaded
        std::ifstream i("Data/Scenes/File.json");
        i >> dialogues;
    }
    
    DialogueTree* tree = new DialogueTree();
    
    for (int i = 0; i < dialogues.size(); i++) {
        if (dialogues.at(i)["DialogueID"] == i) { // find right dialogue
            buildTree(tree, i, 0);
        }
    }
    
    dialogue->setDialogueTree(tree);
}
