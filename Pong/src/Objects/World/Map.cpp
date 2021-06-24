//
//  Map.cpp
//  Pong
//
//  Created by Joseph Gu on 6/24/21.
//

#include "Map.hpp"
#include <vector>
#include "AssetManager.hpp"

void Map::init() {
    shader.init("Shaders/MapVertexShader.vs", "Shaders/MapFragmentShader.fs");
    Vertex a,b,c,d;
    a = {glm::vec3(-100,0,-100),glm::vec3(0,1,0),glm::vec2(0,0)};
    b = {glm::vec3(100,0,-100),glm::vec3(0,1,0),glm::vec2(1,0)};
    c = {glm::vec3(100,0,100),glm::vec3(0,1,0),glm::vec2(1,1)};
    d = {glm::vec3(-100,0,100),glm::vec3(0,1,0),glm::vec2(0,1)};
    std::vector<Vertex> mapVertices = {a,b,c,d
    };
    
    std::vector<GLuint> mapIndices = {
        0, 1, 2,
        2, 3, 0
    };
    std::vector<Texture> mapTextures;
    Texture texture = {loadTexture(TEX_EMPTY),"blah","blah"};
    mapTextures.push_back(texture);
    mesh.setVertexData(mapVertices, mapIndices, mapTextures);
}

Shader& Map::getShader() {
    return shader;
}
