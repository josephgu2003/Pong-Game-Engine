//
//  LifeTime.hpp
//  Pong
//
//  Created by Joseph Gu on 2/17/22.
//

#ifndef LifeTime_hpp
#define LifeTime_hpp

#include <stdio.h>
#include "Component.hpp"
#include "Watch.hpp"
#include "GraphicsComponent.hpp"

template<typename T>
class LifeTime : public Component {
    Watch watch; 
    float duration; 
    float maxDuration;
public:
    LifeTime(T& t, float duration_) : Component(t) {
        watch.resetTime();
        duration = duration_;
        maxDuration = duration_;
        updatePriority = 1.0f;  
    } 
      
    void tick() override {
        if (duration <= 0.0f) {
            static_cast<T*>(actor)->destroy();
            if (GraphicsComponent* gc = static_cast<T*>(actor)->template getComponent<GraphicsComponent>()) {
                gc->setUniform<float>("duration", 0.0f);
            } 
        } else { 
            duration -= watch.getTime(); 
            watch.resetTime();
            if (GraphicsComponent* gc = static_cast<T*>(actor)->template getComponent<GraphicsComponent>()) {
                float ratio = duration / maxDuration;
                gc->setUniform<float>("duration", ratio);
            }
        }
        
    }
};
#endif /* LifeTime_hpp */
