//
//  Ball.cpp
//  Pong
//
//  Created by Joseph Gu on 5/1/21.
//

#include "Ball.hpp"
#include "AssetManager.hpp"

void Ball::loadModel() {
    model = loadModels(MOD_PHOENIX);
    light = {true, 1.5f};
    fiery.init(0.012, posVec+glm::vec3(-0.5,0.6,-0.2), 0.5, 0.3, 0.6, 500, 1);
}

ParticleEffect& Ball::getParticleEffect() {
    return fiery;
}
