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

template <typename T>
class Componentable {
private:
    std::vector<std::shared_ptr<Component<T>>> components;
public:
    Componentable() {
        
    }
    
    void tick() {
        for (int i = 0; i < components.size(); i++) {
            components.at(i)->tick();
        }
    }
     
    virtual void addComp(const std::shared_ptr<Component<T>>& comp) {
        
        if (components.size() == 0) {
            std::shared_ptr<Component<T>> comp_ = comp;
            components.push_back(std::move(comp_));
            return;
        }
        for (int i = 0; i < components.size(); i++) {
            if (components.at(i)->getType() == comp->getType()) {
                components.erase(components.begin()+i); //!
                std::shared_ptr<Component<T>> comp_ = comp;
                components.insert(components.begin()+i,std::move(comp_));
                return;
            }
            else if (i == (components.size()-1)) {
                std::shared_ptr<Component<T>> comp_ = comp;

                components.push_back(std::move(comp_));
            }
            else if (components.at(i)->getType() < comp->getType()) {
                continue;
            }

            else {
                std::shared_ptr<Component<T>> comp_ = comp;
                components.insert(components.begin()+i,std::move(comp_));
                return;
            }
        }
    }

    void deleteComp(CompType type) {
        for (int i = 0; i < components.size(); i++) {
            if (components.at(i)->getType() == type) {
                components.erase(components.begin()+i);
            }
        }
    }

    template <typename compType>
    compType* getComponent()
    {
        for (int i = 0 ; i < components.size(); i++) {
            if (dynamic_cast<compType*>(components.at(i).get()) != NULL) {
                return static_cast<compType*>(components.at(i).get());
            }
        }
            return nullptr; // make a null component or something
 
    }
    
    
    template <typename compType>
    bool hasComponent()
    {
        for (int i = 0 ; i < components.size(); i++) {
            auto x = dynamic_cast<compType*>(components.at(i).get()) ;
            if (x != NULL) {
                return true;
            }
        }
        return false;
    }
    
    bool hasComponent(CompType ct) {
        for (int i = 0; i < components.size(); i++) {
            if (components.at(i)->getType() == ct) {
                return true;
            }
        }
        return false;
    }
};

//option 1 : template, now compentable can reject wrong types
// option 2 : enum, compenentable can reject wrong types by casting
// option 3 : child classes

#endif /* Componentable_hpp */
