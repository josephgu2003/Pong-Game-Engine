//
//  WorldChunk.cpp
//  Pong
//
//  Created by Joseph Gu on 10/30/21.
//

#include "WorldChunk.hpp"
#include "VertexLoader.hpp"
#include "Renderer.hpp"
#include "Shader.hpp" 
#include "AssetManager.hpp"

int MapChunk::counter = 0;

MapChunk::MapChunk(const unsigned short* heightMap, Material& mat, int mapWidth, int mapHeight, int indexX_, int indexY_, glm::vec2 originPos_, glm::vec3 scaling_) : GraphicsObject(DRAW_OPAQUE) {
    shader->init("Shaders/TerrainVertex.vs", "Shaders/TerrainFragmentShader.fs"); 
    Renderer::bindShaderUniblock(shader, DistanceFog); 
    VertexLoader::loadMapChunk(heightMesh, heightMap,mapWidth,mapHeight, indexX_, indexY_, originPos_, scaling_, VAO, VBO, EBO, numIndices);
    scaling = scaling_;   
    originPos =  originPos_;
    setSingularMaterial(mat);
    Renderer::bindShaderUniblock(shader, ViewProj); 
    Renderer::bindShaderUniblock(shader, Lights);
    indexX = indexX_;
    indexY = indexY_;
    dummy = false;
    counter++;
}     
        
void MapChunk::draw(Renderer* r) {  
    r->renderTerrain(this); 
}
    
float MapChunk::getHeightAt(glm::vec2 xz) {
    xz = xz - originPos; 
    glm::vec2 pos = xz; 
    xz.x = xz.x / scaling.x; 
    xz.y = xz.y / scaling.z; 
     
    //xz now in local space and in vertex coords : 1 is one vertex space, etc.
      
    int topLeftX = std::floor(xz.x); 
    int topLeftZ = std::floor(xz.y);
    // error check?
    if (topLeftX < 0 || topLeftX < 0 || topLeftX >= CHUNK_DIM_PXLS || topLeftZ >= CHUNK_DIM_PXLS) {
        return 0;  
    }
    glm::vec3 v1; // top left or bottom rigth
    glm::vec3 v2 = glm::vec3((topLeftX+1)*scaling.x, heightMesh[topLeftX+1][topLeftZ], topLeftZ*scaling.z); // top right
    glm::vec3 v3 = glm::vec3(topLeftX*scaling.x, heightMesh[topLeftX][topLeftZ+1], (topLeftZ+1)*scaling.z); // bottom left
     
    if (std::fmod(xz.x,1) <= (1.0f - std::fmod(xz.y,1))) { // top triangle
        v1 = glm::vec3(scaling.x*topLeftX, heightMesh[topLeftX][topLeftZ], topLeftZ*scaling.z);
    } else {
        v1 = glm::vec3((topLeftX+1)*scaling.x, heightMesh[topLeftX+1][topLeftZ+1], (topLeftZ+1)*scaling.z); 
    }
    float det = (v2.z - v3.z) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.z - v3.z);
    float l1 = ((v2.z - v3.z) * (pos.x - v3.x) + (v3.x - v2.x) * (pos.y - v3.z)) / det;
    float l2 = ((v3.z - v1.z) * (pos.x - v3.x) + (v1.x - v3.x) * (pos.y - v3.z)) / det;
    float l3 = 1.0f - l1 - l2;
    return l1 * v1.y + l2 * v2.y + l3 * v3.y;
    
} 
 
int MapChunk::getIndexX() {
    return indexX; 
}
int MapChunk::getIndexY() {
    return indexY;
}

MapChunk::~MapChunk() {
    counter--;
}
   
