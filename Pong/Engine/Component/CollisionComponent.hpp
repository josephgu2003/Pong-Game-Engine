//
//  CollisionComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 1/12/22.
//

#ifndef CollisionComponent_hpp
#define CollisionComponent_hpp

#include <stdio.h>
#include "Component.hpp"
#include "Subject.hpp"
#include "Observer.hpp"
#include <functional>
#include "Positionable.hpp"

struct AxisBounds { // relative to Positionable center point
    float min;
    float max;
    AxisBounds(float max_, float min_) {
        min = min_;
        max = max_;
    }
    AxisBounds() {
         
    }
};

class Positionable;

class CollisionComponent;

typedef std::function<void(CollisionComponent*, CollisionComponent*)> CollisionCallback;

// AABB collision : https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
enum Axis {
    X,
    Y,
    Z
};
class CollisionComponent : public Component, public Subject, public Observer, public std::enable_shared_from_this<CollisionComponent> { //needs to notify collision system if this has moved,
private:
    CollisionCallback onCollide;
    AxisBounds xAxisRelative;
    AxisBounds yAxisRelative;
    AxisBounds zAxisRelative;
    AxisBounds xAxisAbsolute;
    AxisBounds yAxisAbsolute;
    AxisBounds zAxisAbsolute;
    Positionable& target;
    bool lockTarget;
    inline void makeAbsoluteBounds(AxisBounds& rel, AxisBounds& abs, float center) {
        abs.max = center + rel.max;
        abs.min = center + rel.min;
    }
public:
    template <typename T>
    CollisionComponent(T& p, AxisBounds x, AxisBounds y, AxisBounds z) : Component(p), target(p) {
        updatePriority = 2;
        lockTarget = false;
        xAxisRelative = x;
        yAxisRelative = y;
        zAxisRelative = z;
        glm::vec3 pos = p.getPos();
        makeAbsoluteBounds(xAxisRelative, xAxisAbsolute, pos.x);
        makeAbsoluteBounds(yAxisRelative, yAxisAbsolute, pos.y);
        makeAbsoluteBounds(zAxisRelative, zAxisAbsolute, pos.z);
        onCollide = [] (CollisionComponent* c, CollisionComponent* cc) {
            
        };
    }
    void setOnCollision(CollisionCallback onCollide_);
    void tick() override;
    void onCollision(CollisionComponent* collided);
    void notify(const Subject &subject, GameEvent ge) override;
    const AxisBounds& getAxisBounds(Axis axis) const;
};

#endif /* CollisionComponent_hpp */
