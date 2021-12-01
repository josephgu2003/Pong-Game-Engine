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
#include "uiFrame.hpp"
#include "Script.hpp"

World::World(Renderer* r) {
    renderer = r;
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
    weather.skyTextureFiles.push_back("Resources/Skybox/right.png");
    weather.skyTextureFiles.push_back("Resources/Skybox/left.png");
    weather.skyTextureFiles.push_back("Resources/Skybox/top.png");
    weather.skyTextureFiles.push_back("Resources/Skybox/bottom.png");
    weather.skyTextureFiles.push_back("Resources/Skybox/back.png");
    weather.skyTextureFiles.push_back("Resources/Skybox/front.png");
}

World::~World() {
}
 
void World::insertActor(const std::shared_ptr<Actor>& actor) {
    activeText = new uiText("", -0.5, -0.8);  // lmfao???
    textFrame = new uiFrame(glm::vec2(-0.7, -0.9), glm::vec2(1.5,0.23), TEX_BLACK_GRADIENT);
    textFrame->insertChild(std::shared_ptr<uiText>(activeText));
    std::shared_ptr<Actor> p = actor;
    allActorPtrs.push_back(std::move(p));
    actor -> setWorld(this); 
}
 
std::vector<std::string>* World::getSkyTextureFiles() {
    return &weather.skyTextureFiles;
}

float* World::getSkyVertices() {
    return &skyVertices[0];
}

Updates World::checkforUpdates() {
    return updates;
}

void World::updateCleared(int i) {
    if (i == 0)
    updates.lightingUpdate = false;
    if (i == 1)
    updates.fogUpdate = false;
    if (i == 3)
    updates.skyUpdate = false;
}
 
void World::setWeather(DirectionalLight dirLight_, float fogDensity_, float fogGradient_, glm::vec3 fogColor_, std::vector<std::string> skyTextureFiles_) {
    weather.dirLight = dirLight_;
    weather.fogDensity = fogDensity_;
    weather.fogGradient = fogGradient_;
    weather.fogColor = fogColor_;
    weather.skyTextureFiles = skyTextureFiles_; 
    updates.lightingUpdate = true;
    updates.fogUpdate = true;
    updates.skyUpdate = true; 
}

Weather World::getWeather() {
    return weather;
}

void World::tick() {
    for(int i = 0; i < allSoundTexts.size(); i++) {
        allSoundTexts.at(i)->duration -= (float) glfwGetTime();
        if (allSoundTexts.at(i)->duration <= 0.0) {
            allSoundTexts.erase(allSoundTexts.begin()+i);
            i -= 1;
            updates.textUpdate = true;
        }
    }
    if (updates.textUpdate) {
        updateActiveText();
    }

    for(int i = 0; i < allCameraPtrs.size(); i++) {
        allCameraPtrs[i]->tick();
    }
    

    for(int i = 0; i < allCameraPtrs.size(); i++) {
        allCameraPtrs[i]->updateVecs();
    }
     
    
    for(int i = 0; i < allScripts.size(); i++) {
        allScripts[i]->tick();
    }

    for(int i = 0; i < allActorPtrs.size(); i++) {
        allActorPtrs[i]->tick();
        auto cc =allActorPtrs[i]->getComponent<CombatComponent>();
        if (cc && cc->QHasAbilities()) {
            abilityManager.handleCombatComp(cc);
        }
    }

    for(auto i = allProps.begin(); i != allProps.end(); i++) {
    //    (*i)->tick();
    }
    
    for (int i = 0; i < allParticleEffects.size(); i++) {
       // glm::vec3 newForce = glm::vec3(0,0,0);
            allParticleEffects[i]->tick();
        
    }
    
    abilityManager.tick();
 
    mapManager.drawChunks(renderer); 
     
    for(auto i = allActorPtrs.begin(); i != allActorPtrs.end(); i++) {
        (*i)->getComponent<GraphicsComponent>()->draw(renderer);
    }
    for(auto i = allProps.begin(); i != allProps.end(); i++) {
    //    (*i)->getComponent<GraphicsComponent>()->draw(renderer);
    }
     
      
    for(auto i = allParticleEffects.begin(); i != allParticleEffects.end(); i++) {
        (*i)->getComponent<PGraphicsComponent>()->draw(renderer);
    }
    if (textFrame)
    textFrame->draw(renderer);
}  

void World::informActorProximity(Actor& actor, float radius) {
    for (int i = 0; i < allActorPtrs.size(); i++) {
        if (allActorPtrs.at(i).get() == &actor) continue;
        if (actor.getDistanceTo(allActorPtrs.at(i).get()) <= radius) {
            std::shared_ptr<Actor> t = (allActorPtrs.at(i));
            actor.getComponent<CombatComponent>()->setBigTarget(t);
        }
    }
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

void World::newSoundText(const std::string& text, const glm::vec3& pos, float duration) {
    std::shared_ptr<SoundText> st = std::make_shared<SoundText>(text, pos, duration);
    allSoundTexts.push_back(std::move(st));
    updates.textUpdate = true;
} 
  
   
void World::updateActiveText() {
    if (allCameraPtrs.size() == 0) return; //spaggetti
    std::string s = "";
    for (int i = 0; i < allSoundTexts.size(); i++) {
        if (glm::length(allSoundTexts.at(i)->pos - allCameraPtrs.at(0)->getPos()) < 15.0)
         s.append(allSoundTexts.at(i)->text + "\n"); 
    } 
    activeText->setText(s);
    updates.textUpdate = false;
}

 
void World::setRenderer(Renderer *renderer_) {
    renderer = renderer_;
}

void World::setMap(const std::string& filePath, int pixelsX, int pixelsY, glm::vec3 scaling) {
    mapManager.setMap(filePath, pixelsX, pixelsY, scaling);
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
    
    for(int i = 0; i < allScripts.size(); i++) {
        setHeightFor(allScripts[i].get());
    } 
 
    for(int i = 0; i < allActorPtrs.size(); i++) {
        setHeightFor(allActorPtrs[i].get());
    }

    for(auto i = allProps.begin(); i != allProps.end(); i++) {
        setHeightFor(i->get());
    } 
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
