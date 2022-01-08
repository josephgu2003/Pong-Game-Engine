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
#include <unordered_map>
#include "MapManager.hpp"
#include "Prop.hpp"
#include "Componentable.hpp"
#include "Behaviour.hpp"
#include "WorldRenderingManager.hpp"
#include "WorldSubSystem.hpp"
 
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
class Actor;
class World;

struct Updates {
    bool lightingUpdate;
    bool fogUpdate;
    bool skyUpdate;
};
 

// map: read from height map, 9 chunks at once, repeat vertices at edges (one vertex is one pixel)
// 4 x 4 chunks, 
typedef std::shared_ptr<WorldSubSystem> (*addSubSystem) (World& w); // do this to avoid storing shared_ptrs of WorldSubSystem - this fancier and less confusing
class World : public Componentable {
private:
   // SoundTextManager soundTextManager;
    WorldRenderingManager worldRenderingManager;
    MapManager mapManager;
    AbilityManager abilityManager;
    Updates updates = {false,false,false};
    Weather weather;
     
    Renderer* renderer = NULL;
 
    std::weak_ptr<Actor> playerHero;
    
    std::vector<std::shared_ptr<Actor>> allActorPtrs;
    std::vector<std::shared_ptr<ParticleSystem>> allParticleEffects;
    std::vector<std::shared_ptr<Camera>> allCameraPtrs;
    std::vector<std::shared_ptr<Prop>> allProps;
    std::vector<std::shared_ptr<Behaviour>> allBehaviours;
    
    static std::vector<addSubSystem> worldSubSystemsTemplate;

    
    void loadChunks(glm::vec3 pos);
    
    inline void insertGraphicsToManager(const std::shared_ptr<Componentable>& c) {
     std::weak_ptr<GraphicsObject> gc;
     if (c->getComponentRef<GraphicsObject>(gc)) {
         worldRenderingManager.insertGraphicsComponent(gc);
     }
    }
    
    void drawAll();
    void tickAll();
public:
    bool blur = false;
    
    
    World(Renderer* r);
    ~World();
    
    
    template <typename T>  
    static void registerSubSystem() {
        addSubSystem callbackFunc = [] (World& w) {
            std::shared_ptr<WorldSubSystem> ptr = std::make_shared<T>(w);
            return ptr;
        };
        worldSubSystemsTemplate.push_back(callbackFunc);
    }

    template <typename T> // hahaha templates are cool even if u coulda done Positionable*
    void insert(const std::shared_ptr<T>& placeable) {
        if (typeid(T) == typeid(Prop)) {
            allProps.push_back(dynamic_pointer_cast<Prop>(placeable));
        }
        if (typeid(T) == typeid(Behaviour)) {
            allBehaviours.push_back(dynamic_pointer_cast<Behaviour>(placeable));
            dynamic_pointer_cast<Behaviour>(placeable)->start();
        } 
        if (typeid(T) == typeid(Actor)) {
            auto actor = dynamic_pointer_cast<Actor>(placeable);
            allActorPtrs.push_back(actor);
            actor-> setWorld(this);
        }
        if (typeid(T) == typeid(ParticleSystem)) {
            allParticleEffects.push_back(dynamic_pointer_cast<ParticleSystem>(placeable));
        }
        if (typeid(T) == typeid(Camera)) {
            allCameraPtrs.push_back(dynamic_pointer_cast<Camera>(placeable));
        }
        if (auto compable = dynamic_pointer_cast<Componentable>(placeable)) {
            insertGraphicsToManager(compable);
        } 
    }
     
    template <typename T>
    void deleteX(T* t) {
        if (typeid(T) == typeid(Behaviour)) {
            for (int i = 0; i < allBehaviours.size(); i++) {
                if (dynamic_cast<Behaviour*>(t) == allBehaviours.at(i).get()) {
                    allBehaviours.erase(allBehaviours.begin()+i);
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
    
    void setMap(const std::string& filePath, glm::vec3 scaling);

    void setRenderer(Renderer* renderer); 
    
    std::vector<std::string>* getSkyTextureFiles();
    float* getSkyVertices();
    
    Updates checkforUpdates();
    void updateCleared(int i);
    
    void setWeather(DirectionalLight dirLight_, float fogDensity_, float fogGradient_, glm::vec3 fogColor_, std::vector<std::string> skyTextureFiles_);
    Weather getWeather();  
     
    void tick(); 
    
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
            if (x.get() == actor) continue;
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
    
    float getHeightAt(glm::vec2 xz);

    void markPlayerHero(const Actor* ph);
    Actor* getPlayerHero();
    
};
  

#endif /* World_hpp */
 
// we want:
// sky - special render
// terrain - heightmap loading, mesh stored 
// sun and moon
// clouds
// weather
