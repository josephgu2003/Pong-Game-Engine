//
//  Flag.cpp
//  Pong
//
//  Created by Joseph Gu on 6/10/21.
//

#include "Flag.hpp"
#include "AssetManager.hpp"

void Flag::loadModel() {
    model = loadModels("Resources/Models/Flag/flag.obj");
    model->meshes->at(0).textures->at(0) = loadTexture("Resources/Models/Flag/yirou'sdrawing.jpg");
}
