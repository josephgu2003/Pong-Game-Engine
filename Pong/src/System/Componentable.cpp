//
//  Componentable.cpp
//  Pong
//
//  Created by Joseph Gu on 7/31/21.
//

#include "Componentable.hpp"

void Componentable::addComp(std::shared_ptr<Component> comp) {
    for (int i = 0; i < components.size(); i++) {
        if (components.at(i).getType() == comp->getType()) {
            components.erase(components.begin()+i);
        }
    }
    components.push_back(std::move(comp));
}

void Componentable::deleteComp(CompType type) {
    for (int i = 0; i < components.size(); i++) {
        if (components.at(i).getType() == type) {
            components.erase(components.begin()+i);
        }
    }
}
