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
#include "ProximityComponent.hpp"

//stores locations of everything

struct mapMesh {
    float* mapFirstVertex;
    int mapVertexCount;
    GLuint* mapFirstIndex;
    int mapIndexCount;
};

struct Weather {
    bool brightnessExists;
    bool particlesExists;
    float brightness;
    ParticleEffect particleEffect;
};

struct Quad {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    float size;
    GLuint texture;
    glm::vec3 pos;
    glm::vec3 rotations; //pitch yaw roll
    glm::vec3 force;
};

class World {
    Model mapModel;
    bool updates = false;
    std::vector <Actor*> allActorPtrs;
    std::vector <ParticleEffect*> allParticleEffects;
    std::vector <Camera*> allCameraPtrs;
    std::vector <Quad*> allQuadPtrs;
    
    std::vector<std::string> skyTextureFiles;
    
    float mapVertices [32] = {0.0};
    GLuint mapIndices [6] = {0};
    
    float skyVertices [108] = {0};
    
    Weather weather;
    
    float globalTime;
    
public:
    bool blur = false;
    World();
    ~World();
    
    void insertCamera(Camera* camera);
    
    void insertActor(Actor* actor);
    
    void insertParticleEffect(ParticleEffect* particleEffect);
    void deleteParticleEffect(ParticleEffect* particleEffect);
    
    void insertQuad(Quad* quad);
    void deleteQuad(Quad* quad);
    
    mapMesh* getMapMesh();
    
    Model& getMap();
    
    int getActorsCount();
    Actor* getNthActor(int n);
    
    std::vector<std::string>* getSkyTextureFiles();
    float* getSkyVertices();
    
    std::vector<ParticleEffect*>* getParticleEffects();
    std::vector<Quad*>* getQuads();
    
    bool checkforUpdates();
    void updateCleared();
    
    void setWeather(bool brightnessExists_,
                    bool particlesExists_,
                    float brightness_);
    Weather getWeather();
    
    void tick();
    
    void informProximityComponent(Actor& actor, ProximityComponent& pc);
};
#endif /* World_hpp */
