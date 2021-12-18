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

#define MAX_SIDE_CHUNKS 3
class Renderer;

class MapManager {
private:
    struct MapDescriptor{
        unsigned short* heightMap = NULL;
        int verticesWide;
        int verticesTall;
        int subdivisionsX;
        int subdivisionsY;
        glm::vec3 scalings;
    } mapDescriptor; 
    struct MapChunks {
        std::unique_ptr<MapChunk> chunks[1+2*MAX_SIDE_CHUNKS][1+2*MAX_SIDE_CHUNKS];
    } mapChunks;
    Material terrainMat;
    inline float vertexDimensionToRealUnits(int vertexDim, float scalingFactor) {
            float realDim = (vertexDim-1) * scalingFactor; // 5 fingers 4 spaces
            return realDim;
    }
public:
    MapManager();
    virtual ~MapManager();
    void setMap(const std::string& filePath, glm::vec3 scaling);
    void loadChunks(glm::vec3 pos);
    float getHeightAt(glm::vec2 xz);
    void drawChunks(Renderer* renderer);
};

#endif /* MapManager_hpp */
