//
//  Componentable.hpp
//  Pong
//
//  Created by Joseph Gu on 7/31/21.
//

#ifndef Componentable_hpp
#define Componentable_hpp

#include <stdio.h>
#include <vector>
#include "Component.hpp"
#include <memory>

class Componentable {
private:
    std::vector<std::shared_ptr<Component>> components;
public:
    void addComp(std::shared_ptr<Component> comp);
    void deleteComp(CompType type);
};

#endif /* Componentable_hpp */
