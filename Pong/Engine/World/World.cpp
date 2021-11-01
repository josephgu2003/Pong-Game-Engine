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
    skyTextureFiles.push_back("Resources/Skybox/right.png");
    skyTextureFiles.push_back("Resources/Skybox/left.png");
    skyTextureFiles.push_back("Resources/Skybox/top.png");
    skyTextureFiles.push_back("Resources/Skybox/bottom.png");
    skyTextureFiles.push_back("Resources/Skybox/back.png");
    skyTextureFiles.push_back("Resources/Skybox/front.png");
}

World::~World() {
    
}


void World::insertCamera(Camera* camera) {
    allCameraPtrs.push_back(camera);
}

void World::insertActor(const std::shared_ptr<Actor>& actor) {
    activeText = new uiText("", -0.8, -0.7);  // lmfao???
    std::shared_ptr<Actor> p = actor;
    allActorPtrs.push_back(std::move(p)); 
    actor -> setWorld(this);
}
 
void World::insertParticleEffect(ParticleSystem* particleEffect) {
    allParticleEffects.push_back(particleEffect);
    particleEffect->setWorld(this);

}

void World::deleteParticleEffect(ParticleSystem *particleEffect) {
    allParticleEffects.erase(std::find(allParticleEffects.begin(), allParticleEffects.end(), particleEffect)); 
    map = new MapChunk(0,0);
} 

 
std::vector<std::string>* World::getSkyTextureFiles() {
    return &skyTextureFiles;
}

float* World::getSkyVertices() {
    return &skyVertices[0];
}

std::vector<ParticleSystem*> World::getParticleEffects() {
    return allParticleEffects;
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
   // updates.quadUpdate = false;
    if (i == 3)
    updates.textUpdate = false;
    if (i == 4)
    updates.lightingUpdate = false; 
}
 
void World::setWeather(DirectionalLight dirLight_, int sky_) {
    weather.dirLight = dirLight_;
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

    for(int i = 0; i < allActorPtrs.size(); i++) {

        allActorPtrs[i]->tick();
        auto cc =allActorPtrs[i]->getComponent<CombatComponent>();
        if (cc && cc->QHasAbilities()) {
            abilityManager.handleCombatComp(cc);
        }
    }
    for(int i = 0; i < allForces.size(); i++) {

        allForces[i]->tick();
    }

    for(int i = 0; i < allCameraPtrs.size(); i++) {
        allCameraPtrs[i]->updateVecs();
    }
    
    for (int i = 0; i < allParticleEffects.size(); i++) {
       // glm::vec3 newForce = glm::vec3(0,0,0);
            allParticleEffects[i]->tick();
        
    }
    abilityManager.tick();
  
if (map)    map->draw(renderer);
      
    for(auto i = allActorPtrs.begin(); i != allActorPtrs.end(); i++) {
        (*i)->getComponent<GraphicsComponent>()->draw(renderer);
    }
     
    for(auto i = allParticleEffects.begin(); i != allParticleEffects.end(); i++) {
        (*i)->getComponent<PGraphicsComponent>()->draw(renderer);
    }
    if (activeText)
    renderer->renderText(activeText);
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
 
bool World::informParticlesForce(ParticleSystem* effect) {
 /**   glm::vec3 uniformstraightforce = glm::vec3(0);
    bool b = false;
    effect->forces.clear();
    for (int i = 0; i < allForces.size(); i++) {
        if (glm::length(effect->getPos()-allForces.at(i)->getPos()) < 5) {
            uniformstraightforce += allForces.at(i)->getUniStraightForce();
            b = true;
            effect->forces.push_back( allForces.at(i));
        }
    }
    effect->setForce(uniformstraightforce);
    return b;
  **/
    return true; 
}


void World::insertForce(Force* force_) {
    allForces.push_back(force_);
}
void World::deleteForce(Force* force_) {
    allForces.erase(std::find(allForces.begin(), allForces.end(), force_), allForces.end());
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
        if (glm::length(allSoundTexts.at(i)->pos - allCameraPtrs.at(0)->getPos()) < 10.0)
         s.append(allSoundTexts.at(i)->text + "\n");
    } 
    activeText->setText(s);
    updates.textUpdate = false;
}

 
void World::setRenderer(Renderer *renderer_) {
    renderer = renderer_;
}
 
