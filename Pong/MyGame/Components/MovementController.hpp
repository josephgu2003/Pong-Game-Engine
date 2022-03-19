//
//  MovementController.hpp
//  Pong
//
//  Created by Joseph Gu on 2/28/22.
//

#ifndef MovementController_hpp
#define MovementController_hpp

#include <stdio.h>
#include "Component.hpp"
#include "Watch.hpp"

class Actor;

enum MovementDirection {
    MOVEDIR_FORWARDS,
    MOVEDIR_BACKWARDS,
    MOVEDIR_RIGHT,
    MOVEDIR_LEFT
};

enum MovementMode {
    MOVEMODE_PARALYZED,
    MOVEMODE_IDLE,
    MOVEMODE_WALK,
    MOVEMODE_RUN
};

static const float walkSpeed = 0.03f;
static const float runSpeed = 0.06f;

class MovementController : public Component {
private:
    MovementMode moveMode;
    float timeLeftOnParalysis = 0.0f;
    Watch watch;
public:
    MovementController(Actor& a);
    void move(MovementDirection dir);
    void setMovementMode(MovementMode mm);
    void paralyze(float duration);
    void makeIdle();
    void tick() override;
};
#endif /* MovementController_hpp */
