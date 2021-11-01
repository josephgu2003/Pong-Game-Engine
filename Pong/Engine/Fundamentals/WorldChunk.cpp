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

MapChunk::MapChunk(int chunkX, int chunkY) : Renderable() { 
    shader->init("Shaders/RegularVertex.vs", "Shaders/SnowFragmentShader.fs");
    VertexLoader::loadMapChunk("Resources/Map/heightmap.png", chunkX, chunkY, 1, VAO, VBO, EBO, numIndices);
    Renderer::bindShaderUniblock(shader, ViewProj);
    Renderer::bindShaderUniblock(shader, Lights);
}   
     
void MapChunk::draw(Renderer* r) {
    r->renderElements(this);
}
 
 
 
