//
//  NameComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 9/9/21.
//

#ifndef NameComponent_hpp
#define NameComponent_hpp

#include <stdio.h>
#include "Component.hpp"
#include <string>

class NameComponent : public Component {
protected:
    std::string commonName;
    std::string idName;
public:
    NameComponent(Actor& actor);
    void init(const std::string& common, const std::string& id);
    virtual void tick() override;
    const std::string& getCommonName();
    const std::string& getIdName();
};

#endif /* NameComponent_hpp */
