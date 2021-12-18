//
//  MapManager.cpp
//  Pong
//
//  Created by Joseph Gu on 11/3/21.
//

#include "MapManager.hpp"
#include "AssetManager.hpp"

MapManager::MapManager() {
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            mapChunks.chunks[i][j] = std::unique_ptr<MapChunk>();
        }
    }
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
    AssetManager::loadTexture("Resources/Map/snow01.png", &terrainMat.diffuse, true);
    AssetManager::loadTexture("Resources/Map/snownorm.png", &terrainMat.normMap, false); 
} 
      

void MapManager::loadChunks(glm::vec3 pos) { // protocol: to avoid gaps between chunks, all chunks include the prior row/column vertices
    glm::vec3 scalings = mapDescriptor.scalings;
    float xdim = vertexDimensionToRealUnits(mapDescriptor.verticesWide, scalings.x);
    float ydim = vertexDimensionToRealUnits(mapDescriptor.verticesTall, scalings.z);
    float xPosLocal = pos.x + xdim / 2.0f;
    float zPosLocal = pos.z + ydim / 2.0f; // x and z interchangeable lol

    int indexX = std::floor(xPosLocal / CHUNK_DIM_PXLS / mapDescriptor.scalings.x);
    int indexY = std::floor(zPosLocal / CHUNK_DIM_PXLS / mapDescriptor.scalings.z);
    
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
            
            mapChunks.chunks[i-iInitial][j-jInitial] = std::make_unique<MapChunk>(mapDescriptor.heightMap, terrainMat, mapDescriptor.verticesWide, mapDescriptor.verticesTall, i, j, originPos, scalings);
        }
    }
}
 
float MapManager::getHeightAt(glm::vec2 xz) {
    MapChunk* cornerChunk = mapChunks.chunks[0][0].get();
    if (!cornerChunk) {
        return 0.0f;
    }
    glm::vec3 scalings = mapDescriptor.scalings; 
    float xdim = vertexDimensionToRealUnits(mapDescriptor.verticesWide, scalings.x);
    float ydim = vertexDimensionToRealUnits(mapDescriptor.verticesTall, scalings.z);
    float cornerX = float(cornerChunk->getIndexX())*float(CHUNK_DIM_PXLS-1)*scalings.x - (xdim / 2.0f);
    float cornerZ = float(cornerChunk->getIndexY())*float(CHUNK_DIM_PXLS-1)*scalings.z - (ydim / 2.0f);
    float localX = xz.x - cornerX;
    float localZ = xz.y - cornerZ;
    int x = std::floor(localX /((CHUNK_DIM_PXLS-1)*scalings.x));
    int y = std::floor(localZ /((CHUNK_DIM_PXLS-1)*scalings.z));
    if (x > 7 || y > 7 || x < 0 || y < 0) {
        return 0.0f; 
    }  
    return mapChunks.chunks[x][y]->getHeightAt(xz);
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
 
  
 
