//
//  World.hpp
//  Pong
//
//  Created by Joseph Gu on 5/3/21.
//

#ifndef World_hpp
#define World_hpp

#include <vector>
#include <iostream>
#include <stdio.h>
#include <string>
#include "Actor.hpp"
#include "Ball.hpp"
#include "PlayerHero.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct mapMesh {
    float* mapFirstVertex;
    int mapVertexCount;
    GLuint* mapFirstIndex;
    int mapIndexCount;
};

class World {
    std::vector <Actor*> allActorPtrs;
    std::vector<float> worldVertices;
    std::vector<GLuint> worldIndices;
    std::vector<std::string> skyTextureFiles;
    float mapVertices [20] = {0.0};
    GLuint mapIndices [6] = {0};
    float skyVertices [108] = {0};
public:
    World();
    ~World();
    
    void insertActor(Actor* actor);
    
    mapMesh* getMapMesh();
    
    int getActorsCount();
    Actor* getNthActor(int n);
    
    std::vector<std::string>* getSkyTextureFiles();
    float* getSkyVertices();
};
#endif /* World_hpp */
