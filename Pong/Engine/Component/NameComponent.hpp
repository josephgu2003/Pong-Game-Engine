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

class Actor; 
class NameComponent : public Component { 
protected:
    std::string name;
public:
    NameComponent(Actor& actor);
    void init(const std::string& name);
    virtual void tick() override;
    const std::string& getName();
    void speak(const std::string& speech, float duration);
};

#endif /* NameComponent_hpp */
