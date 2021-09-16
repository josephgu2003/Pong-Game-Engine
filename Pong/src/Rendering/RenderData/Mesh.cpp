//
//  Mesh.cpp
//  Pong
//
//  Created by Joseph Gu on 5/14/21.
//

#include "Mesh.hpp"

Mesh::Mesh() {
    
}

Mesh::Mesh(std::vector<std::shared_ptr<AnyVertex>>& vertices_,
     std::vector<GLuint>& indices_,
           TextureMaps& textures_, VertexType vertexType_) {
    vertices = vertices_;
    indices = indices_;
    textures = textures_;
    vertexType = vertexType_;
} 

