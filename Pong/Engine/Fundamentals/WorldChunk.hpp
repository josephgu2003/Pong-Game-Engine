//
//  WorldChunk.hpp
//  Pong
//
//  Created by Joseph Gu on 10/30/21.
//

#ifndef WorldChunk_hpp
#define WorldChunk_hpp

#include <stdio.h>
#include "GraphicsObject.hpp"

#define CHUNK_DIM_PXLS 256
 
class Renderer;

class MapChunk : public GraphicsObject {
private:
    float heightMesh[CHUNK_DIM_PXLS][CHUNK_DIM_PXLS]; // to find height at point here,
    glm::vec3 scaling;
    glm::vec2 transformToLocal;
public :
    MapChunk(const std::string& src, int indexX, int indexY, glm::vec2 transformToLocal_, glm::vec3 scaling);
    virtual void draw(Renderer* r);
    float getHeightAt(glm::vec2 xz);
};
  

#endif /* WorldChunk_hpp */
