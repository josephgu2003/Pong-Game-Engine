//
//  TheLake.cpp
//  Pong
//
//  Created by Joseph Gu on 2/12/22.
//

#include "TheLake.hpp"
#include "PropFactory.hpp"
#include "World.hpp"

std::vector<std::string> lakeCrew = {
    "Floro"
};

std::vector<std::string> lakeprereqs = {};
  
TheLake::TheLake(World* world, bool completed) : Script(world, lakeCrew, 30.0f, completed, "By the Lake", lakeprereqs) {
     
}

void TheLake::act() {
    Actor* floro = getActorNamed("Floro");
    
    switch (step) { 
        case 0: {
            PropFactory pf;
            auto ray = pf.makeProp(PROP_LIGHTRAY);
            ray->setPos(glm::vec3(12, 5, 0));
            world->insert<Prop>(ray);
            step++; 
            break;
        }
            
        default:
            break;
    }
}
