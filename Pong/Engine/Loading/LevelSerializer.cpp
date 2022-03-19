//
//  LevelSerializer.cpp
//  Pong
//
//  Created by Joseph Gu on 12/30/21.
//

#include "LevelSerializer.hpp" 
#include <fstream>

void LevelSerializer::loadLevelWorlds(GameLevel *g){
   nlohmann::json saveFile;
   std::ifstream inStream(GAMELVLS_PATH);
   inStream >> saveFile; 
    
   nlohmann::json lvlSave = saveFile.find(g->getName()).value(); 

   for (int i = 0; i < lvlSave.size(); i++) { // iterate over objects {worldid : ....}, ...
       nlohmann::json& worlddata = lvlSave[i];
       World* w = &(g->getWorld(worlddata.find("WorldID").value()));
       nlohmann::json& arrayEntities = worlddata.find("Data").value();
        
       for (int x = 0; x < arrayEntities.size(); x++) {
           nlohmann::json& entity = arrayEntities[x];
           convertPrimitive(w, entity["Type"], entity["FactoryEnum"], glm::vec3(entity["Position"][0],entity["Position"][1],entity["Position"][2]));
       }
   }
} 
 
