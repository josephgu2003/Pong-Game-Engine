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
#include "BossProximityComponent.hpp"

Ball::Ball() {}
void Ball::loadModel() {
    model = loadModels(MOD_HOODY);
    std::vector<GLuint> newMaps = {loadTexture(TEX_INKPAPER)};
    for (int i = 0; i<model->getMeshes()->size(); i ++) {
        model->setMeshTexture(i, DIFFUSE, newMaps);
    }
    BossProximityComponent* component = new BossProximityComponent();
    components.push_back(component);
}

void Ball::tick() {
    Actor::tick();
}
