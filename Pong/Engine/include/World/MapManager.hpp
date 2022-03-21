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
#include <queue>

#define MAX_SIDE_CHUNKS 5
#define CHUNKS_PER_ROW (1+2*MAX_SIDE_CHUNKS)
class Renderer; 
class MapManager {
private:
    void loadChunksAbstraction(const glm::vec3& pos, bool oneAtATime);
    struct ChunkIndices {
        int indexX;
        int indexY;
    };
    struct MapDescriptor{
        unsigned short* heightMap = NULL;
        int verticesWide;
        int verticesTall;
        int subdivisionsX;
        int subdivisionsY;
        glm::vec3 scalings;
    } mapDescriptor; 
    struct MapChunks {
        MapChunk chunks[1+2*MAX_SIDE_CHUNKS][1+2*MAX_SIDE_CHUNKS];
        ChunkIndices centerChunk;
    } mapChunks;
    bool loadingUnloadedChunks;
    Material terrainMat; 
    inline float vertexDimensionToRealUnits(int vertexDim, float scalingFactor) {
            float realDim = (vertexDim-1) * scalingFactor; // 5 fingers 4 spaces
            return realDim;
    }
    inline void getChunkIndicesFromWorldCoords(const glm::vec3& pos, int& indexX, int& indexY) {
        glm::vec3 scalings = mapDescriptor.scalings;
        float xdim = vertexDimensionToRealUnits(mapDescriptor.verticesWide, scalings.x);
        float ydim = vertexDimensionToRealUnits(mapDescriptor.verticesTall, scalings.z);
        float xPosLocal = pos.x + xdim / 2.0f;
        float zPosLocal = pos.z + ydim / 2.0f; // x and z interchangeable lol

        indexX = std::floor(xPosLocal / CHUNK_DIM_PXLS / mapDescriptor.scalings.x);
        indexY = std::floor(zPosLocal / CHUNK_DIM_PXLS / mapDescriptor.scalings.z);
    }
     
    void recenterMapChunks(int newXIndex, int newYIndex);

    template <typename FunctorI, typename FunctorJ>
    void recenterMapChunksAbstraction(int deltaI, int deltaJ, FunctorI functorI, FunctorJ functorJ) {
        
        auto isWithinBounds = [] (int i, int j) {
            return (i >= 0 && i < 1+2*MAX_SIDE_CHUNKS) && (j >= 0 && j < 1+2*MAX_SIDE_CHUNKS);
        };
        
        for (int j = 0; j < CHUNKS_PER_ROW; j++) {
            for (int i = 0; i < CHUNKS_PER_ROW; i++) {
                int iPos = functorI(i);
                int jPos = functorJ(j);
                MapChunk& mc = mapChunks.chunks[iPos][jPos];
                if (isWithinBounds(iPos + deltaI, jPos + deltaJ) && !mc.dummy) {
                    mapChunks.chunks[iPos + deltaI][jPos + deltaJ] = std::move(mapChunks.chunks[iPos][jPos]);
                    mc.dummy = true;
                }
            }
        }
         
    }

public:
    MapManager();
    virtual ~MapManager();
    void setMap(const std::string& filePath, glm::vec3 scaling);
    void loadChunks(const glm::vec3& pos); 
    float getHeightAt(const glm::vec2& xz);
    void tick(const glm::vec3& pos);
    void drawChunks(Renderer* renderer);
};

#endif /* MapManager_hpp */
