//
//  World.hpp
//  Pong
//
//  Created by Joseph Gu on 5/3/21.
//

#ifndef World_hpp
#define World_hpp

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Particle.hpp"
#include "Camera.hpp"
#include "DirectionalLight.hpp"
#include "AbilityManager.hpp" 
#include <memory>
#include "Batch.hpp"
#include "uiText.hpp" 
#include "Watch.hpp"
#include <unordered_map>
#include "MapManager.hpp"
#include "Prop.hpp"
#include "Script.hpp"


typedef std::vector<std::shared_ptr<Actor>> ActorList;

struct Weather {
    DirectionalLight dirLight;
    float fogDensity;
    float fogGradient;
    glm::vec3 fogColor;
    std::vector<std::string> skyTextureFiles;
};


class Renderer;
class MapChunk;
class uiFrame;

struct Updates {
    bool lightingUpdate;
    bool fogUpdate;
    bool textUpdate;
    bool skyUpdate;
};
 
struct SoundText {
    std::string text;
    glm::vec3 pos;
    float duration;
    SoundText(const std::string& text_, const glm::vec3& pos_, float duration_) {
        text = text_;
        pos = pos_;
        duration = duration_;
    }
};

class Actor;

// map: read from height map, 9 chunks at once, repeat vertices at edges (one vertex is one pixel)
// 4 x 4 chunks, 

class World {
private: 
    MapManager mapManager;
    uiText* activeText = NULL;
    uiFrame* textFrame = NULL; 
    Updates updates = {false,false, false};
    Renderer* renderer = NULL;
      
    std::weak_ptr<Actor> playerHero;
    
    std::vector<std::shared_ptr<Actor>> allActorPtrs;
    std::vector<std::shared_ptr<ParticleSystem>> allParticleEffects;
    std::vector<std::shared_ptr<Camera>> allCameraPtrs;
    std::vector<std::shared_ptr<Prop>> allProps;
    std::vector<std::shared_ptr<Script>> allScripts;
    std::vector<std::shared_ptr<SoundText>> allSoundTexts;
    
    float skyVertices [108] = {0};
    
    Weather weather;
     
    Watch globalTime;
    
    AbilityManager abilityManager;
    void updateActiveText();
public:
    bool blur = false;
    World();
    ~World(); 
    
    template <typename T> // hahaha templates are cool even if u coulda done Positionable*
    void insert(const std::shared_ptr<T>& placeable) {
        if (typeid(T) == typeid(Prop)) {
            allProps.push_back(dynamic_pointer_cast<Prop>(placeable));
        } 
        if (typeid(T) == typeid(Actor)) {
            allActorPtrs.push_back(dynamic_pointer_cast<Actor>(placeable));
        }
        if (typeid(T) == typeid(ParticleSystem)) {
            allParticleEffects.push_back(dynamic_pointer_cast<ParticleSystem>(placeable));
        }
        if (typeid(T) == typeid(Camera)) {
            allCameraPtrs.push_back(dynamic_pointer_cast<Camera>(placeable));
        }
        if (typeid(T) == typeid(Script)) {
            allScripts.push_back(dynamic_pointer_cast<Script>(placeable));
        }
    }
     
    template <typename T>
    void deleteX(T* t) {
        if (typeid(T) == typeid(Script)) {
            for (int i = 0; i < allScripts.size(); i++) {
                if (dynamic_cast<Script*>(t) == allScripts.at(i).get()) {
                    allScripts.erase(allScripts.begin()+i);
                } 
            }
        }
        if (typeid(T) == typeid(Prop)) {
            for (int i = 0; i < allProps.size(); i++) {
                if (dynamic_cast<Prop*>(t) == allProps.at(i).get()) {
                    allProps.erase(allProps.begin()+i);
                }
            }
        }
        if (typeid(T) == typeid(Actor)) {
            for (int i = 0; i < allActorPtrs.size(); i++) {
                if (dynamic_cast<Actor*>(t) == allActorPtrs.at(i).get()) {
                    allActorPtrs.erase(allActorPtrs.begin()+i);
                }
            }
        }
        if (typeid(T) == typeid(ParticleSystem)) {
            for (int i = 0; i < allParticleEffects.size(); i++) {
                if (dynamic_cast<ParticleSystem*>(t) == allParticleEffects.at(i).get()) {
                    allParticleEffects.erase(allParticleEffects.begin()+i);
                }
            }
        }
        if (typeid(T) == typeid(Camera)) {
            for (int i = 0; i < allCameraPtrs.size(); i++) {
                if (dynamic_cast<Camera*>(t) == allCameraPtrs.at(i).get()) {
                    allCameraPtrs.erase(allCameraPtrs.begin()+i);
                }
            } 
        }
    }
    
    void setMap(const std::string& filePath, int pixelsX, int pixelsY, glm::vec3 scaling);

    void setRenderer(Renderer* renderer);
    
    void insertActor(const std::shared_ptr<Actor>& actor);
    
    std::vector<std::string>* getSkyTextureFiles();
    float* getSkyVertices();
    
    //std::vector<ParticleSystem*> getParticleEffects();
    
    Updates checkforUpdates();
    void updateCleared(int i);
    
    void setWeather(DirectionalLight dirLight_, float fogDensity_, float fogGradient_, glm::vec3 fogColor_, std::vector<std::string> skyTextureFiles_);
    Weather getWeather();  
     
    void tick();
    
    void informActorProximity(Actor& actor, float radius);
    
    const ActorList getNearActorsWith(Actor* actor, CompType ct)
    {
        glm::vec3 pos = actor->getPos();
        ActorList al;
        for (int i = 0 ; i < allActorPtrs.size(); i++) {
            auto x = allActorPtrs.at(i);
            if (x.get()==actor) continue;
            if (glm::length(pos - x->getPos()) < 10.0) {
                if (x->hasComponent(ct)) {
                    al.push_back(x);
                } 
            }
        }
            return al; // make a null component or something

    } 
    bool getNearestActorWith(Actor* actor, CompType ct, Actor*& nearest)
    {  
        bool hasNear = false;
        for (int i = 0 ; i < allActorPtrs.size(); i++) {
            auto x = allActorPtrs.at(i);
            if (x.get()==actor) continue;
            if (x->hasComponent(ct)) {
                if (nearest == NULL || actor->getDistanceTo(x.get()) < actor->getDistanceTo(nearest)) {
                    nearest = x.get();
                    hasNear = true;
                }
            }
        }
        return hasNear;
    }

    std::shared_ptr<Actor> getActorNamed(const std::string& name);
    
    void newSoundText(const std::string& text, const glm::vec3& pos, float duration);
    
    float getHeightAt(glm::vec2 xz);

    void setPlayerHero(const std::shared_ptr<Actor>& ph);
};
 

#endif /* World_hpp */
 
// we want:
// sky - special render
// terrain - heightmap loading, mesh stored 
// sun and moon
// clouds
// weather
