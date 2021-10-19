//
//  VertexLoader.hpp
//  Pong
//
//  Created by Joseph Gu on 10/16/21.
//

#ifndef VertexLoader_hpp
#define VertexLoader_hpp

#include <stdio.h>
#include "VertexData.hpp"

struct TextureMaps;

namespace VertexLoader {
    void loadTextData(const std::string& s, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, TextureMaps& map);
    void setupVAOAttribs(VertexType vt);
    void load2DQuadData(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, glm::vec2 dimensions, glm::vec2 position);
};

#endif /* VertexLoader_hpp */
