//
//  World.cpp
//  Pong
//
//  Created by Joseph Gu on 5/3/21.
//

#include "World.hpp"


World::World() {
    Ball* ball = new Ball();
    
    ball->loadModel();
    
    allActorPtrs.push_back(ball);
    float mapVerticesCopy [] = {
        -100, 0, -100, 0, 0,
        100, 0, -100, 5, 0,
        100, 0, 100, 5, 5,
        -100, 0, 100, 0, 5
    };
    GLuint mapIndicesCopy [] = {
        0, 1, 2,
        2, 3, 0
    };
    float skyVerticesCopy[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    memcpy(mapVertices, mapVerticesCopy, sizeof(mapVerticesCopy));
    memcpy(mapIndices, mapIndicesCopy, sizeof(mapIndicesCopy));
    memcpy(skyVertices, skyVerticesCopy, sizeof(skyVerticesCopy));
    skyTextureFiles.push_back("Resources/skybox/right.jpg");
    skyTextureFiles.push_back("Resources/skybox/left.jpg");
    skyTextureFiles.push_back("Resources/skybox/top.jpg");
    skyTextureFiles.push_back("Resources/skybox/bottom.jpg");
    skyTextureFiles.push_back("Resources/skybox/front.jpg");
    skyTextureFiles.push_back("Resources/skybox/back.jpg");
}

World::~World() {
    
}

void World::insertActor(Actor* actor) {
    allActorPtrs.push_back(actor);
}

mapMesh* World::getMapMesh() {
    mapMesh* mesh = new mapMesh; //new mesh?
    mesh->mapFirstVertex = &mapVertices[0];
    mesh->mapVertexCount = sizeof(mapVertices)/sizeof(mapVertices[0]);
    mesh->mapFirstIndex = &mapIndices[0];
    mesh->mapIndexCount = sizeof(mapIndices)/sizeof(mapIndices[0]);
    return mesh;
}

int World::getActorsCount() {
    return allActorPtrs.size();
}
Actor* World::getNthActor(int n) {
    return allActorPtrs[n];
}
std::vector<std::string>* World::getSkyTextureFiles() {
    return &skyTextureFiles;
}
float* World::getSkyVertices() {
    return &skyVertices[0];
}
