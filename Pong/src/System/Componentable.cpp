//
//  Componentable.cpp
//  Pong
//
//  Created by Joseph Gu on 7/31/21.
//

#include "Componentable.hpp"
Componentable::Componentable() {
    
} 
void Componentable::init(Actor* actor_, World* world_) {
    actor = actor_;
    world = world_;
}

void Componentable::tick() {
    for (int i = 0; i < components.size(); i++) {
        components.at(i)->tick(*actor, *world);
    }
}

void Componentable::addComp(const std::shared_ptr<Component>& comp) {

    if (components.size() == 0) {
        std::shared_ptr<Component> comp_ = comp;
        components.push_back(std::move(comp_));
        return; 
    } 
    for (int i = 0; i < components.size(); i++) {
        if (components.at(i)->getType() == comp->getType()) {
            CompType t = comp->getType();
            CompType c = components.at(i)->getType();
            components.erase(components.begin()+i); //!
            std::shared_ptr<Component> comp_ = comp;
            components.insert(components.begin()+i,std::move(comp_));
            return;
        }
        else if (i == (components.size()-1)) {
            std::shared_ptr<Component> comp_ = comp;

            components.push_back(std::move(comp_));
        }
        else if (components.at(i)->getType() < comp->getType()) {
            continue;
        }

        else {
            std::shared_ptr<Component> comp_ = comp;
            components.insert(components.begin()+i,std::move(comp_));
            return;
        }
    }
}

void Componentable::deleteComp(CompType type) {
    for (int i = 0; i < components.size(); i++) {
        if (components.at(i)->getType() == type) {
            components.erase(components.begin()+i);
        }
    }
}

std::shared_ptr<Component> Componentable::getComp(CompType type) {
    for (int i = 0; i < components.size(); i++) {
        if (components.at(i)->getType() == type) {
            return components.at(i);
        }
    }
    std::shared_ptr<Component> comp;
    return comp;
}
