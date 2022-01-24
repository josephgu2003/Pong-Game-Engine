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
#include "CombatComponent.hpp"
#include "NameComponent.hpp"
#include "Actor.hpp"
#include "Renderable.hpp"
#include "Renderer.hpp"
#include "PGraphicsComponent.hpp" 
#include "WorldChunk.hpp"
#include "Script.hpp"
 
std::vector<addSubSystem> World::worldSubSystemsTemplate;

World::World(Renderer* r) { 
    distanceFog = WorldFog(r);
    renderer = r; 
    for (auto i = worldSubSystemsTemplate.begin(); i != worldSubSystemsTemplate.end(); i++) {
        addComp((*i)(*this));
    }
    collisionSystem = std::make_shared<CollisionSystem>(); 
}


World::~World() {
    if (allCameraPtrs.size() > 0) {
        allCameraPtrs.at(0)->setActor(nullptr); 
    }
}  

Updates World::checkforUpdates() {
    return updates;
}

void World::updateCleared(int i) {
    if (i == 0)
    updates.lightingUpdate = false; 
    if (i == 1)
    updates.fogUpdate = false;
    if (i == 2)
    updates.skyUpdate = false;
}

Atmosphere& World::getAtmosphere() {
    return atmosphere;
}
void World::drawAll() {
    atmosphere.draw(renderer);
    
    mapManager.drawChunks(renderer);
        
    worldRenderingManager.drawAll(renderer);
    
    for (auto i = components.begin(); i != components.end(); i++) {
        if (auto wss = dynamic_pointer_cast<WorldSubSystem>((*i))) wss->drawAll(renderer);
    }
        
   // soundTextManager.drawAll(renderer);
}

void World::tickAll() {
    Componentable::tick();
    
    for (auto i = allBehaviours.begin(); i != allBehaviours.end(); i++) {
        if ((*i)->isRunning()) {
        (*i)->tick();
        } else {
            deleteX<Behaviour>((*i).get());
            i--;
        }
    }

    for(int i = 0; i < allActorPtrs.size(); i++) {
        allActorPtrs[i]->tick();
        auto cc =allActorPtrs[i]->getComponent<CombatComponent>();
        if (cc && cc->QHasAbilities()) {
            abilityManager.handleCombatComp(cc);
        }
    }  
    for(int i = 0; i < allCameraPtrs.size(); i++) {
        allCameraPtrs[i]->tick();
    }
 
    for(int i = 0; i < allCameraPtrs.size(); i++) {
        allCameraPtrs[i]->updateVecs();
    }

    for(auto i = allProps.begin(); i != allProps.end(); i++) {
        (*i)->tick();
    }
    
    for (int i = 0; i < allParticleEffects.size(); i++) {
            allParticleEffects[i]->tick();
    }
    
    abilityManager.tick();
    if (auto ph = playerHero.lock()) {
     //   soundTextManager.tick(ph->getPos());
        mapManager.tick(ph->getPos()); 
    }

}

void World::tick() {
    tickAll();
    
    drawAll();
}
 
std::shared_ptr<Actor> World::getActorNamed(const std::string& name) {
    for (auto i = allActorPtrs.begin(); i != allActorPtrs.end(); i++) {
        auto namec = i->get()->getComponent<NameComponent>();
        if (namec) {
            if (namec->getName() == name) {
                return *i;
            } 
        }
    }
    std::shared_ptr<Actor> dummy = std::make_shared<Actor>();
    return dummy;
}

 
void World::setRenderer(Renderer *renderer_) {
    renderer = renderer_;
}

void World::setMap(const std::string& filePath, glm::vec3 scaling) {
    mapManager.setMap(filePath, scaling);
    if (auto x = playerHero.lock()) {
        loadChunks(x->getPos()); 
    }
} 
 
float World::getHeightAt(glm::vec2 xz) {
    return mapManager.getHeightAt(xz); 
}
  
void World::markPlayerHero(const Actor* ph) {
    for (auto x : allActorPtrs) { 
        if (x.get() == ph) {
            playerHero = x; 
        }
    }  
}
 
Actor* World::getPlayerHero() {
    if (auto x = playerHero.lock()) {
        return x.get(); 
    }
    return nullptr;  
}

void World::loadChunks(glm::vec3 pos) {
    mapManager.loadChunks(pos);
    
    auto setHeightFor = [&] (Positionable* positionable) {
        glm::vec3 currentPos = positionable->getPos();
        glm::vec2 newPos = glm::vec2(currentPos.x, currentPos.z);
        positionable->setPosY(mapManager.getHeightAt(newPos));
    }; 

    for(int i = 0; i < allActorPtrs.size(); i++) {
        setHeightFor(allActorPtrs[i].get());
    }

    for(auto i = allProps.begin(); i != allProps.end(); i++) {
        setHeightFor(i->get());
    } 
}

std::weak_ptr<Camera> World::getCameraRef() {
    std::weak_ptr<Camera> camref = allCameraPtrs.at(0);
    return camref;
}

void World::setDirectionalLight(const DirectionalLight& dl_) {
    if (renderer) {
        renderer->updateLights(dl_);
    } 
    dl = dl_;
}

const DirectionalLight& World::getDirectionalLight() {
    return dl;
}

WorldFog& World::getDistanceFog() {
    return distanceFog;
}

// next steps:
// world objects class hierarchy
// terrain height for chars
// terrain normals
 
// actors - actorcomp
// lights
// forces
// camera
// terrain chunk
// particle effect - particle comp
// trees grass etc 
 
// components shared between things:
// static mesh
// dynamic mesh
// physics?
// animation
// graphics

