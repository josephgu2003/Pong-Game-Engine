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
    for (int i = 0; i < 1+2*MAX_SIDE_CHUNKS; i++) {
        for (int j = 0; j < 1+2*MAX_SIDE_CHUNKS; j++) {
            mapChunks.chunks[i][j] = std::shared_ptr<MapChunk>();
         //   mapChunks.proxyChunks[i][j] = std::shared_ptr<MapChunk>();
        }
    }
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
      

void MapManager::loadNextUnloadedChunk(const glm::vec3& pos) {
    int indexX;

    int indexY;
    
    getChunkIndicesFromWorldCoords(pos,indexX, indexY);
    
    mapChunks.centerChunk.indexX = indexX;
    mapChunks.centerChunk.indexY = indexY;
    
    if (indexX > (mapDescriptor.subdivisionsX-4)) {
        indexX = (mapDescriptor.subdivisionsX-4);
    }
    if (indexY > (mapDescriptor.subdivisionsY-4)) {
        indexY = (mapDescriptor.subdivisionsY-4);
    }
    int iInitial = indexX - MAX_SIDE_CHUNKS;
    int jInitial = indexY - MAX_SIDE_CHUNKS;
    
    glm::vec3 scalings = mapDescriptor.scalings;
    float xdim = vertexDimensionToRealUnits(mapDescriptor.verticesWide, scalings.x);
    float ydim = vertexDimensionToRealUnits(mapDescriptor.verticesTall, scalings.z);
    
    for (int i = iInitial; i <= indexX + MAX_SIDE_CHUNKS; i++) {
        for (int j = jInitial; j <= indexY + MAX_SIDE_CHUNKS; j++) {
            if (!mapChunks.chunks[i-iInitial][j-jInitial].get()) {
                glm::vec2 originPos = glm::vec2(-0.5*xdim + i * (CHUNK_DIM_PXLS-1) * scalings.x, -0.5*ydim + j *(CHUNK_DIM_PXLS-1)*scalings.z);
                
                mapChunks.chunks[i-iInitial][j-jInitial] = std::make_shared<MapChunk>(mapDescriptor.heightMap, terrainMat, mapDescriptor.verticesWide, mapDescriptor.verticesTall, i, j, originPos, scalings);
                return;// cc dont need origniPos, scaling all afterwards
            }
             
        }
    }
    loadingUnloadedChunks = false;
}

void MapManager::loadChunks(const glm::vec3& pos) { // protocol: to avoid gaps between chunks, all chunks include the prior row/column vertices
    int indexX;
    
    int indexY;
    
    glm::vec3 scalings = mapDescriptor.scalings;
    
    float xdim = vertexDimensionToRealUnits(mapDescriptor.verticesWide, scalings.x);
    float ydim = vertexDimensionToRealUnits(mapDescriptor.verticesTall, scalings.z); 
    
    getChunkIndicesFromWorldCoords(pos,indexX, indexY);
    
    mapChunks.centerChunk.indexX = indexX;
    mapChunks.centerChunk.indexY = indexY;
    
    if (indexX > (mapDescriptor.subdivisionsX-4)) {
        indexX = (mapDescriptor.subdivisionsX-4);
    }
    if (indexY > (mapDescriptor.subdivisionsY-4)) {
        indexY = (mapDescriptor.subdivisionsY-4);
    }
    int iInitial = indexX - MAX_SIDE_CHUNKS;
    int jInitial = indexY - MAX_SIDE_CHUNKS;
    
    for (int i = iInitial; i <= indexX + MAX_SIDE_CHUNKS; i++) {
        for (int j = jInitial; j <= indexY + MAX_SIDE_CHUNKS; j++) {
            
            glm::vec2 originPos = glm::vec2(-0.5*xdim + i * (CHUNK_DIM_PXLS-1) * scalings.x, -0.5*ydim + j *(CHUNK_DIM_PXLS-1)*scalings.z);
            
            mapChunks.chunks[i-iInitial][j-jInitial] = std::make_shared<MapChunk>(mapDescriptor.heightMap, terrainMat, mapDescriptor.verticesWide, mapDescriptor.verticesTall, i, j, originPos, scalings);
            
        } 
    }
}
 
float MapManager::getHeightAt(const glm::vec2& xz) {
    MapChunk* middleChunk = mapChunks.chunks[MAX_SIDE_CHUNKS][MAX_SIDE_CHUNKS].get(); // use middle to calculate the position at [0][0] in case [0][0] is unloaded
    if (!middleChunk) {
        return 0.0f;
    }
    glm::vec3 scalings = mapDescriptor.scalings;
    float xdim = vertexDimensionToRealUnits(mapDescriptor.verticesWide, scalings.x);
    float ydim = vertexDimensionToRealUnits(mapDescriptor.verticesTall, scalings.z);
    float cornerX = -(xdim / 2.0f) + float(middleChunk->getIndexX())*float(CHUNK_DIM_PXLS-1)*scalings.x-MAX_SIDE_CHUNKS*float(CHUNK_DIM_PXLS-1)*scalings.x;
    float cornerZ =  -(ydim / 2.0f) + float(middleChunk->getIndexY())*float(CHUNK_DIM_PXLS-1)*scalings.z-MAX_SIDE_CHUNKS*float(CHUNK_DIM_PXLS-1)*scalings.z;
     
    float localX = xz.x - cornerX;
    float localZ = xz.y - cornerZ;
    int x = std::floor(localX /((CHUNK_DIM_PXLS-1)*scalings.x));
    int y = std::floor(localZ /((CHUNK_DIM_PXLS-1)*scalings.z));
    if (x > 1+2*MAX_SIDE_CHUNKS|| y > 1+2*MAX_SIDE_CHUNKS || x < 0 || y < 0) {
        return 0.0f; 
    }
    if (mapChunks.chunks[x][y].get()) {
        return mapChunks.chunks[x][y]->getHeightAt(xz);
    } else {
        return 0.0f;
    }
} 
 
void MapManager::drawChunks(Renderer* renderer) {
        for (int i = 0; i < (MAX_SIDE_CHUNKS*2 + 1); i++) {
            for (int j = 0; j < (MAX_SIDE_CHUNKS*2 + 1); j++) {
                if (auto x = mapChunks.chunks[i][j].get()) {
                    x->draw(renderer);
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
        loadNextUnloadedChunk(pos);
    }
}

void MapManager::recenterMapChunks(int newXIndex, int newYIndex) {
    int deltaX = mapChunks.centerChunk.indexX - newXIndex; // example: center point has moved to the right, all indices need to change to smaller values.
    int deltaY = mapChunks.centerChunk.indexY - newYIndex;
    
    auto isWithinBounds = [] (int i) { 
        if (i >= 0 && i < 1+2*MAX_SIDE_CHUNKS) {
            return true;
        } else {
            return false;
        }
    }; 
    
    std::shared_ptr<MapChunk> proxy[1+2*MAX_SIDE_CHUNKS][1+2*MAX_SIDE_CHUNKS];
    
    // fill proxy with all chunks that need to be transferred
    for (int x = 0; x < 1+2*MAX_SIDE_CHUNKS; x++) {
        for (int y = 0; y < 1+2*MAX_SIDE_CHUNKS; y++) {
            int newX = x+deltaX;
            int newY = y+deltaY;
            if (isWithinBounds(newX) && isWithinBounds(newY)) {
                proxy[newX][newY] = mapChunks.chunks[x][y];
            }
        }
    }
    
    // fill mapChunks with all proxy's transferrable chunks, delete unneeded ones
    for (int x = 0; x < 1+2*MAX_SIDE_CHUNKS; x++) {
        for (int y = 0; y < 1+2*MAX_SIDE_CHUNKS; y++) {
            if (auto chunk = proxy[x][y].get()) {
                mapChunks.chunks[x][y] = proxy[x][y];
            } else {
                mapChunks.chunks[x][y].reset();
            }
        }
    }
    
    mapChunks.centerChunk.indexX = newXIndex;
    mapChunks.centerChunk.indexY = newYIndex;
}
