//
//  MapManager.cpp
//  Pong
//
//  Created by Joseph Gu on 11/3/21.
//

#include "MapManager.hpp"

MapManager::MapManager() {
    mapChunks.activeChunk = std::unique_ptr<MapChunk>();
    for (int i = 0; i < 8; i++) {
        mapChunks.sideChunks[i] = std::unique_ptr<MapChunk>();
    }
}

void MapManager::setMap(const std::string& filePath, int pixelsX, int pixelsY, glm::vec3 scaling) {
    mapDescriptor.filePath = filePath;
    mapDescriptor.subdivisionsX = pixelsX / CHUNK_DIM_PXLS;
    mapDescriptor.subdivisionsY = pixelsY / CHUNK_DIM_PXLS;
    assert(fmod(mapDescriptor.subdivisionsX, 1) == 0);
    assert(fmod(mapDescriptor.subdivisionsY, 1) == 0);
    mapDescriptor.xdim = (pixelsX-1) * scaling.x; // 5 fingers, 4 spaces
    mapDescriptor.ydim = (pixelsY-1) * scaling.z; // ^^  
    mapDescriptor.scalings = scaling;
}
     
void MapManager::loadChunks(glm::vec3 pos) { // protocol: to avoid gaps between chunks, all chunks include the prior row/column vertices 
    float xPosLocal = pos.x + mapDescriptor.xdim / 2.0f;
    float zPosLocal = pos.z + mapDescriptor.ydim / 2.0f;
    int indexX = 0;
    int indexY = 0;
    float smallChunkWidth = (CHUNK_DIM_PXLS-1)*mapDescriptor.scalings.x; // cuz the leftmost chunks are a bit narrower
    float smallChunkHeight = (CHUNK_DIM_PXLS-1)*mapDescriptor.scalings.z;
    glm::vec2 transformToLocal = glm::vec2(-0.5*mapDescriptor.xdim, -0.5*mapDescriptor.ydim);
      
    if (xPosLocal > smallChunkWidth) {
        indexX = 1+std::floor((xPosLocal-smallChunkWidth)/ CHUNK_DIM_PXLS / mapDescriptor.scalings.x); // 5 fingers, 4 spaces, but an extra space cuz extra vertices
        transformToLocal.x += (indexX-1)*(CHUNK_DIM_PXLS)*mapDescriptor.scalings.x+smallChunkWidth;
    }
    if (zPosLocal > smallChunkHeight) { 
        indexY = 1+std::floor((zPosLocal-smallChunkHeight)/ CHUNK_DIM_PXLS / mapDescriptor.scalings.z);
        transformToLocal.y += (indexX-1)*(CHUNK_DIM_PXLS)*mapDescriptor.scalings.z+smallChunkHeight;
    } 
     
    mapChunks.activeChunk.reset(new MapChunk(mapDescriptor.filePath, indexX, indexY, transformToLocal, mapDescriptor.scalings));
       
} 

float MapManager::getHeightAt(glm::vec2 xz) {
    if (mapChunks.activeChunk.get() == nullptr) {
        return 0; // lol
    }
    return mapChunks.activeChunk->getHeightAt(xz);
} 
 
void MapManager::drawChunks(Renderer* renderer) {
    if (mapChunks.activeChunk.get()) {
        mapChunks.activeChunk->draw(renderer);
    }
    for (int i = 0; i < 8; i++) {
        if (mapChunks.sideChunks[i].get())
        mapChunks.sideChunks[i]->draw(renderer);
    }
}
 
