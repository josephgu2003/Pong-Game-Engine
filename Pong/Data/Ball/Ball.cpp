//
//  Ball.cpp
//  Pong
//
//  Created by Joseph Gu on 5/1/21.
//

#include "Ball.hpp"
#include "AssetManager.hpp"
#include "FallingLetters.hpp"
#include "Speech.hpp"
#include "PhysicsComponent.hpp"
#include "CombatComponent.hpp"

Ball::Ball() {
    std::cout << "oof2";
        posVec.x = 0.0f;
        posVec.y = 0.0f;
        posVec.z = 0.0f;
        dirVec = glm::vec3(0.0f, 0.0f, -1.0f);
        velVec = glm::vec3(0.0f, 0.0f, 0.0f);
        rightVec = glm::cross(dirVec,glm::vec3(0,1,0));
    std::shared_ptr<Component> pc = std::make_shared<PhysicsComponent>(false);
    addComp(pc);
    state = STATE_FLYING;
    distribution = std::uniform_int_distribution<int>(1,1000);
}

void Ball::init(int i) {
    Actor::init(i);
}

void Ball::tick() {
    Actor::tick();
}
