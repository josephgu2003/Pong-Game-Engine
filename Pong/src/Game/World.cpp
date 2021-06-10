//
//  World.cpp
//  Pong
//
//  Created by Joseph Gu on 5/3/21.
//

#include "World.hpp"
#include <iostream>
#include <stdio.h>
#include "AssetManager.hpp"

World::World() {
    Model* mapModel_ = loadModels("Resources/Map/mountains-free/source/1.obj");
    mapModel = *mapModel_;
    mapModel_->~Model();
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
    skyTextureFiles.push_back("Resources/Skybox/right.png");
    skyTextureFiles.push_back("Resources/Skybox/left.png");
    skyTextureFiles.push_back("Resources/Skybox/top.png");
    skyTextureFiles.push_back("Resources/Skybox/bottom.png");
    skyTextureFiles.push_back("Resources/Skybox/back.png");
    skyTextureFiles.push_back("Resources/Skybox/front.png");
    setWeather(true, false, 0.6);
    globalTime = 0;
}

World::~World() {
    
}

void World::insertCamera(Camera* camera) {
    allCameraPtrs.push_back(camera);
}

void World::insertActor(Actor* actor) {
    allActorPtrs.push_back(actor);
    actor -> setWorld(this);
}

void World::insertParticleEffect(ParticleEffect* particleEffect) {
    allParticleEffects.push_back(particleEffect);
    updates = true;
}

void World::deleteParticleEffect(ParticleEffect *particleEffect) {
    allParticleEffects.erase(std::find(allParticleEffects.begin(), allParticleEffects.end(), particleEffect), allParticleEffects.end());
    updates = true;
}

void World::insertQuad(Quad* quad) {
    allQuadPtrs.push_back(quad);
    updates = true;
}

void World::deleteQuad(Quad *quad) {
    allQuadPtrs.erase(std::find(allQuadPtrs.begin(), allQuadPtrs.end(), quad), allQuadPtrs.end());
    updates = true;
}

mapMesh* World::getMapMesh() {
    mapMesh* mesh = new mapMesh; //new mesh?
    mesh->mapFirstVertex = &mapVertices[0];
    mesh->mapVertexCount = sizeof(mapVertices)/sizeof(mapVertices[0]);
    mesh->mapFirstIndex = &mapIndices[0];
    mesh->mapIndexCount = sizeof(mapIndices)/sizeof(mapIndices[0]);
    return mesh;
}

Model& World::getMap() {
    return mapModel;
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

std::vector<ParticleEffect*>* World::getParticleEffects() {
    return &allParticleEffects;
}
std::vector<Quad*>* World::getQuads() {
    return &allQuadPtrs;
}

bool World::checkforUpdates() {
    return updates;
}

void World::updateCleared() {
    updates = false;
}

void World::setWeather(bool brightnessExists_,
                bool particlesExists_,
                float brightness_) {
    weather = {
        brightnessExists_,
                        particlesExists_,
                        brightness_
    };
}

Weather World::getWeather() {
    return weather;
}

void World::tick() {
    globalTime += glfwGetTime();
  //  weather.brightness = 0.5*(sin(globalTime)+1.3);
    for(int i = 0; i < allActorPtrs.size(); i++) {
        allActorPtrs[i]->tick();
    }
    for(int i = 0; i < allCameraPtrs.size(); i++) {
        allCameraPtrs[i]->tick();
    }
    for(int i = 0; i < allParticleEffects.size(); i++) {
        allParticleEffects[i]->tick();
    }
}
 
