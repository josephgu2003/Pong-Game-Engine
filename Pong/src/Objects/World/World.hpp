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
#include "Actor.hpp"
#include "Ball.hpp"
#include "PlayerHero.hpp"
#include "Particle.hpp"
#include "Camera.hpp"
#include "Model.hpp"
#include "Map.hpp"
#include "DirectionalLight.hpp"
#include "Numberable.hpp"

#define ACTOR_UPDATE 0
#define PARTICLE_UPDATE 1
#define QUAD_UPDATE 2
#define TEXT_UPDATE 3
//stores locations of everything

struct Weather {
    DirectionalLight dirLight;
};

struct Quad {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    float size;
    float alpha;
    GLuint texture;
    glm::vec3 pos;
    glm::vec3 rotations; //pitch yaw roll
    glm::vec3 force;
};

struct Updates {
    bool actorUpdate;
    bool particleUpdate;
    bool quadUpdate;
    bool textUpdate;
};
class World : public Numberable {
    Map* map;
    Updates updates = {false,false,false,false};
    std::vector <Actor*> allActorPtrs;
    std::vector <ParticleEffect*> allParticleEffects;
    std::vector <Camera*> allCameraPtrs;
    std::vector <Quad*> allQuadPtrs;
    std::string activeText;

    std::vector<std::string> skyTextureFiles;
    
    float skyVertices [108] = {0};
    
    Weather weather;
    
    float globalTime;
    
public:
    bool blur = false;
    World();
    ~World();
    
    void setMap(Map& map);
    void insertCamera(Camera* camera);
    
    void insertActor(Actor* actor);
    
    void insertParticleEffect(ParticleEffect* particleEffect);
    void deleteParticleEffect(ParticleEffect* particleEffect);
    
    void insertQuad(Quad* quad);
    void deleteQuad(Quad* quad);
    
    void setActiveText(const std::string& string);
    std::string getActiveText();
    
    Map& getMap();
    
    int getActorsCount();
    Actor* getNthActor(int n);
    
    std::vector<std::string>* getSkyTextureFiles();
    float* getSkyVertices();
    
    std::vector<ParticleEffect*> getParticleEffects();
    std::vector<Quad*>* getQuads();
    
    Updates checkforUpdates();
    void updateCleared(int i);
    
    void setWeather(DirectionalLight dirLight);
    Weather getWeather();
    
    void tick();
    
    void informActorProximity(Actor& actor, float radius);
};


#endif /* World_hpp */
