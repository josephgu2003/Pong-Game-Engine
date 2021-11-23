//
//  MapManager.hpp
//  Pong
//
//  Created by Joseph Gu on 11/3/21.
//

#ifndef MapManager_hpp
#define MapManager_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <memory>
#include "WorldChunk.hpp"

class Renderer;

class MapManager {
private:
    struct MapDescriptor{
        std::string filePath;
        float xdim;
        float ydim;
        int subdivisionsX;
        int subdivisionsY;
        glm::vec3 scalings;
    } mapDescriptor; 
    struct MapChunks {
        std::unique_ptr<MapChunk> activeChunk;
        std::unique_ptr<MapChunk> sideChunks[8];
    } mapChunks;
public:
    MapManager();
    void setMap(const std::string& filePath, int pixelsX, int pixelsY, glm::vec3 scaling);
    void loadChunks(glm::vec3 pos);
    float getHeightAt(glm::vec2 xz);
    void drawChunks(Renderer* renderer);
};

#endif /* MapManager_hpp */
