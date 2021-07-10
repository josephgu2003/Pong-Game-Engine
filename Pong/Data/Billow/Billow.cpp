//
//  Billow.cpp
//  Pong
//
//  Created by Joseph Gu on 6/10/21.
//

#include "Billow.hpp"
#include "AssetManager.hpp"

void Billow::loadModel() {
    model = loadModels("Resources/Models/Flag/flag.obj");
    Texture texture = {
        loadTexture("Resources/Models/Flag/yirou'sdrawing.jpg"), "texture_diffuse", "a"
    };
    model->meshes.at(0).textures.push_back(texture);
}

