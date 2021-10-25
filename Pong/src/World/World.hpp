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
#include "MapObject.hpp"
#include "DirectionalLight.hpp"
#include "Force.hpp"
#include "AbilityManager.hpp" 
#include <memory>
#include "Batch.hpp"
#include "uiText.hpp"
 
#define ACTOR_UPDATE 0
#define PARTICLE_UPDATE 1
#define QUAD_UPDATE 2
#define TEXT_UPDATE 3
#define LIGHTING_UPDATE 3

typedef std::vector<std::shared_ptr<Actor>> ActorList;

struct Weather {
    DirectionalLight dirLight;
    int sky;
};

class Renderer;

struct Updates {
    bool actorUpdate;
    bool particleUpdate;
    bool textUpdate;
    bool lightingUpdate;
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
    uiText* activeText = NULL;
    MapObject* map = NULL;
    Updates updates = {false,false,false,false};
    Renderer* renderer = NULL;
      
    std::vector <std::shared_ptr<Actor>> allActorPtrs;
    std::vector <ParticleSystem*> allParticleEffects;
    std::vector <MapObject*> allMapObjPtrs;
    std::vector <Camera*> allCameraPtrs;
    std::vector <Force*> allForces;
    std::vector<std::shared_ptr<SoundText>> allSoundTexts;
    std::vector<std::string> skyTextureFiles;
    
    float skyVertices [108] = {0};
    
    Weather weather;
     
    float globalTime;
    
    AbilityManager abilityManager;
    void updateActiveText();
public:
    bool blur = false;
    World();
    ~World();
    
    void setMap(MapObject& map);
    void insertMapObj(MapObject* map);
    
    void insertCamera(Camera* camera);
    void setRenderer(Renderer* renderer);
    
    void insertActor(const std::shared_ptr<Actor>& actor);
    
    void insertParticleEffect(ParticleSystem* particleEffect);
    void deleteParticleEffect(ParticleSystem* particleEffect);
    
    void insertForce(Force* force_);
    void deleteForce(Force* force_);

    MapObject& getMap();
    MapObject& getMapObjs();
    
    int getActorsCount();
    std::shared_ptr<Actor> getNthActor(int n);
    
    std::vector<std::string>* getSkyTextureFiles();
    float* getSkyVertices();
    
    std::vector<ParticleSystem*> getParticleEffects();
    
    Updates checkforUpdates();
    void updateCleared(int i);
    
    void setWeather(DirectionalLight dirLight, int sky);
    Weather getWeather();
     
    void tick();
    
    void informActorProximity(Actor& actor, float radius);
    bool informParticlesForce(ParticleSystem* effect);
    
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
    std::shared_ptr<Actor> getActorNamed(const std::string& name);
    
    void newSoundText(const std::string& text, const glm::vec3& pos, float duration);
    
    void drawText(Renderer* r);
};
 

#endif /* World_hpp */
