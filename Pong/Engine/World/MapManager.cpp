//
//  MapManager.cpp
//  Pong
//
//  Created by Joseph Gu on 11/3/21.
//

#include "MapManager.hpp"
#include "AssetManager.hpp"
/**
 std::mutex dumpingChunks;
 std::mutex loadM;
 
 bool loadingChunks = false;**/

MapManager::MapManager() {
    loadingUnloadedChunks = false;
}

void MapManager::setMap(const std::string& filePath, glm::vec3 scaling) {
    int pixelsX = 0;
    int pixelsY = 0;
    AssetManager::loadHeightMap(filePath, mapDescriptor.heightMap, &pixelsX, &pixelsY);
    mapDescriptor.subdivisionsX = std::ceil(pixelsX / CHUNK_DIM_PXLS);
    mapDescriptor.subdivisionsY = std::ceil(pixelsY / CHUNK_DIM_PXLS);
    assert(fmod(mapDescriptor.subdivisionsX, 1) == 0);
    assert(fmod(mapDescriptor.subdivisionsY, 1) == 0);
    mapDescriptor.verticesWide = pixelsX;
    mapDescriptor.verticesTall = pixelsY;
    mapDescriptor.scalings = scaling;
    AssetManager::loadTexture("Resources/Map/snow.png", &terrainMat.normMap, false);
}     


void MapManager::loadChunksAbstraction(const glm::vec3& pos, bool oneAtATime) {
    int indexX;
    
    int indexY;
    
    getChunkIndicesFromWorldCoords(pos,indexX, indexY);
    
    mapChunks.centerChunk.indexX = indexX;
    mapChunks.centerChunk.indexY = indexY;
    
    if (indexX > (mapDescriptor.subdivisionsX-MAX_SIDE_CHUNKS-1)) {
        indexX = (mapDescriptor.subdivisionsX-MAX_SIDE_CHUNKS-1);
    }
    if (indexY > (mapDescriptor.subdivisionsY-MAX_SIDE_CHUNKS-1)) {
        indexY = (mapDescriptor.subdivisionsY-MAX_SIDE_CHUNKS-1);
    }
    int iInitial = indexX - MAX_SIDE_CHUNKS;
    int jInitial = indexY - MAX_SIDE_CHUNKS;
    
    glm::vec3 scalings = mapDescriptor.scalings;
    float xdim = vertexDimensionToRealUnits(mapDescriptor.verticesWide, scalings.x);
    float ydim = vertexDimensionToRealUnits(mapDescriptor.verticesTall, scalings.z);
    
    for (int i = iInitial; i <= indexX + MAX_SIDE_CHUNKS; i++) {
        for (int j = jInitial; j <= indexY + MAX_SIDE_CHUNKS; j++) {
            MapChunk& mc = mapChunks.chunks[i - iInitial][j - jInitial];
            if (mc.dummy) {
                glm::vec2 originPos = glm::vec2(-0.5*xdim + i * (CHUNK_DIM_PXLS-1) * scalings.x, -0.5*ydim + j *(CHUNK_DIM_PXLS-1)*scalings.z); 
                mapChunks.chunks[i - iInitial][j - jInitial] =  std::move(MapChunk(mapDescriptor.heightMap, terrainMat, mapDescriptor.verticesWide, mapDescriptor.verticesTall, i, j, originPos, scalings));
                if (oneAtATime) {
                    return;// cc dont need origniPos, scaling all afterwards
                }
            }
              
        } 
    }
    loadingUnloadedChunks = false;
    printf("%i", GraphicsObject::nonHolders);
}

void MapManager::loadChunks(const glm::vec3& pos) { // protocol: to avoid gaps between chunks, all chunks include the prior row/column vertices
    loadChunksAbstraction(pos, false);
}
 
float MapManager::getHeightAt(const glm::vec2& xz) {
    MapChunk& middleChunk = mapChunks.chunks[MAX_SIDE_CHUNKS][MAX_SIDE_CHUNKS]; // use middle to calculate the position at [0][0] in case [0][0] is unloaded
    if (middleChunk.dummy) {
        return 0.0f;
    }
    glm::vec3 scalings = mapDescriptor.scalings;
    float xdim = vertexDimensionToRealUnits(mapDescriptor.verticesWide, scalings.x);
    float ydim = vertexDimensionToRealUnits(mapDescriptor.verticesTall, scalings.z);
    float cornerX = -(xdim / 2.0f) + float(middleChunk.getIndexX())*float(CHUNK_DIM_PXLS-1)*scalings.x-MAX_SIDE_CHUNKS*float(CHUNK_DIM_PXLS-1)*scalings.x;
    float cornerZ =  -(ydim / 2.0f) + float(middleChunk.getIndexY())*float(CHUNK_DIM_PXLS-1)*scalings.z-MAX_SIDE_CHUNKS*float(CHUNK_DIM_PXLS-1)*scalings.z;
    
    float localX = xz.x - cornerX;
    float localZ = xz.y - cornerZ;
    int x = std::floor(localX /((CHUNK_DIM_PXLS-1)*scalings.x));
    int y = std::floor(localZ /((CHUNK_DIM_PXLS-1)*scalings.z));
    if (x > 1+2*MAX_SIDE_CHUNKS|| y > 1+2*MAX_SIDE_CHUNKS || x < 0 || y < 0) {
        return 0.0f;
    }
    
    MapChunk& mc = mapChunks.chunks[x][y];
    if (!mc.dummy) {
        return mc.getHeightAt(xz);
    } else {
        return 0.0f;
    }
} 

void MapManager::drawChunks(Renderer* renderer) {
    for (int j = 0; j < (MAX_SIDE_CHUNKS*2 + 1); j++) {
        for (int i = 0; i < (MAX_SIDE_CHUNKS*2 + 1); i++) {
            MapChunk& mc = mapChunks.chunks[i][j];
            if (!mc.dummy) {
                mc.draw(renderer);
            } 
        }
    }
} 

MapManager::~MapManager() {
    if (mapDescriptor.heightMap) {
        delete[] mapDescriptor.heightMap;
    }
}

void MapManager::tick(const glm::vec3& pos) {
    int indexX, indexY;
    getChunkIndicesFromWorldCoords(pos, indexX, indexY);
    
    if (indexX != mapChunks.centerChunk.indexX || indexY != mapChunks.centerChunk.indexY) {
             recenterMapChunks(indexX, indexY);
          loadingUnloadedChunks = true;
        // read into proxy, draw cant draw when proxy turns into main chunks,
        // load can't load
    }
    if (loadingUnloadedChunks) {
        loadChunksAbstraction(pos, true);
    }
}

void MapManager::recenterMapChunks(int newXIndex, int newYIndex) {
    // MOVE ROW BY ROW
    // what happens if called multiple times
    
    int deltaX = mapChunks.centerChunk.indexX - newXIndex; // example: center point has moved to the right, all indices need to change to smaller values.
    int deltaY = mapChunks.centerChunk.indexY - newYIndex;

    auto shiftNormal = [] (int x) {
        return x;
    };
    
    auto shiftReverse = [] (int x) {
        return CHUNKS_PER_ROW - x - 1;
    };
    
    if (deltaX > 0) {
        // shift starting from right elements per row
        if (deltaY > 0) {
            //shift starting from bottom rows to top rows
            recenterMapChunksAbstraction(deltaX, deltaY, shiftReverse, shiftReverse);
        } else {
            recenterMapChunksAbstraction(deltaX, deltaY, shiftReverse, shiftNormal);
        }
    } else {
        if (deltaY > 0) {
            //shift starting from bottom rows to top rows
            recenterMapChunksAbstraction(deltaX, deltaY, shiftNormal, shiftReverse);
        } else {
            recenterMapChunksAbstraction(deltaX, deltaY, shiftNormal, shiftNormal);
        }
    }
    
    
    mapChunks.centerChunk.indexX = newXIndex;
    mapChunks.centerChunk.indexY = newYIndex;
}
