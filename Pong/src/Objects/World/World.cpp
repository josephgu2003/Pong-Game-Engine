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
    memcpy(skyVertices, skyVerticesCopy, sizeof(skyVerticesCopy));
    
 /** skyTextureFiles.push_back("Resources/Skybox/Screen Shot 2021-06-30 at 3.45.04 PM.png");
    skyTextureFiles.push_back("Resources/Skybox/Screen Shot 2021-06-30 at 3.45.04 PM.png");
    skyTextureFiles.push_back("Resources/Skybox/Screen Shot 2021-06-30 at 3.45.04 PM.png");
    skyTextureFiles.push_back("Resources/Skybox/Screen Shot 2021-06-30 at 3.45.04 PM.png");
    skyTextureFiles.push_back("Resources/Skybox/Screen Shot 2021-06-30 at 3.45.04 PM.png");    skyTextureFiles.push_back("Resources/Skybox/Screen Shot 2021-06-30 at 3.45.04 PM.png");**/
    
    
    globalTime = 0;
}

World::~World() {
    
}

void World::setMap(MapObject &map_) {
    map = &map_;
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
    particleEffect->setWorld(this);
    updates.particleUpdate = true;
}

void World::deleteParticleEffect(ParticleEffect *particleEffect) {
    allParticleEffects.erase(std::find(allParticleEffects.begin(), allParticleEffects.end(), particleEffect), allParticleEffects.end());
    updates.particleUpdate = true;
}

void World::insertQuad(Quad* quad) {
    allQuadPtrs.push_back(quad);
    updates.quadUpdate = true;
}

void World::deleteQuad(Quad *quad) {
    allQuadPtrs.erase(std::find(allQuadPtrs.begin(), allQuadPtrs.end(), quad), allQuadPtrs.end());
    updates.quadUpdate = true;
}

void World::setActiveText(const std::string& string) {
    activeText = string;
    updates.textUpdate = true;
}
std::string World::getActiveText() {
    return activeText;
}

MapObject& World::getMap() {
    return *map;
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

std::vector<ParticleEffect*> World::getParticleEffects() {
    return allParticleEffects;
}
std::vector<Quad*>* World::getQuads() {
    return &allQuadPtrs;
}

Updates World::checkforUpdates() {
    return updates;
}

void World::updateCleared(int i) {
    if (i == 0)
    updates.actorUpdate = false;
    if (i == 1)
    updates.particleUpdate = false;
    if (i == 2)
    updates.quadUpdate = false;
    if (i == 3)
    updates.textUpdate = false;
    if (i == 4)
    updates.lightingUpdate = false;
}

void World::setWeather(DirectionalLight dirLight_, int sky_) {
    weather.dirLight = dirLight_;
    if (sky_ == 0) {
        skyTextureFiles.clear();
        skyTextureFiles.push_back("Resources/Skybox/right.png");
        skyTextureFiles.push_back("Resources/Skybox/left.png");
        skyTextureFiles.push_back("Resources/Skybox/top.png");
        skyTextureFiles.push_back("Resources/Skybox/bottom.png");
        skyTextureFiles.push_back("Resources/Skybox/back.png");
        skyTextureFiles.push_back("Resources/Skybox/front.png");
    }
}

Weather World::getWeather() {
    return weather;
}

void World::tick() {
    globalTime += glfwGetTime();
    map->tick();

  //  weather.brightness = 0.5*(sin(globalTime)+1.3);
    for(int i = 0; i < allCameraPtrs.size(); i++) {
        allCameraPtrs[i]->tick();
    }

    for(int i = 0; i < allActorPtrs.size(); i++) {

        allActorPtrs[i]->tick();
    }
    for(int i = 0; i < allForces.size(); i++) {

        allForces[i]->tick();
    }

    for(int i = 0; i < allCameraPtrs.size(); i++) {
        allCameraPtrs[i]->updateVecs();
    }
    
    for (int i = 0; i < allParticleEffects.size(); i++) {
        glm::vec3 newForce = glm::vec3(0,0,0);
        if (allQuadPtrs.size()>0) {
            for (int j = 0; j < allQuadPtrs.size(); j++) {
                Quad& quad = *allQuadPtrs.at(j);
                ParticleEffect& pEffect = *allParticleEffects.at(i);
                glm::vec3 diff = quad.pos - pEffect.posVec;
                if (glm::length(diff) < 2) {
                    newForce += quad.force;
                }
                allParticleEffects[i]->setForce(newForce);
                allParticleEffects[i]->tick();
            }
        }
        else {
            allParticleEffects[i]->tick();
        }
    }
}

void World::informActorProximity(Actor& actor, float radius) {
    
    std::vector<Actor*> allOtherActors = allActorPtrs;
    allOtherActors.erase(std::remove(allOtherActors.begin(), allOtherActors.end(), &actor));
    for (int i = 0; i < allOtherActors.size(); i++) {
        float distance = glm::length(actor.getPos()-allOtherActors.at(i)->getPos());
        if (distance <= radius) {
            actor.biggestTarget = allOtherActors.at(i);
        }
    }
}

bool World::informParticlesForce(ParticleEffect* effect) {
    glm::vec3 uniformstraightforce = glm::vec3(0);
    bool b = false;
    effect->forces.clear();
    for (int i = 0; i < allForces.size(); i++) {
        if (glm::length(effect->posVec-allForces.at(i)->getPos()) < 5) {
            uniformstraightforce += allForces.at(i)->getUniStraightForce();
            b = true;
            effect->forces.push_back( allForces.at(i));
        }
    }
    effect->setForce(uniformstraightforce);
    return b;
}

void World::insertMapObj(MapObject* map) {
    allMapObjPtrs.push_back(map);
}

void World::insertForce(Force* force_) {
    allForces.push_back(force_);
}
void World::deleteForce(Force* force_) {
    allForces.erase(std::find(allForces.begin(), allForces.end(), force_), allForces.end());
}
