//
//  Mesh.cpp
//  Pong
//
//  Created by Joseph Gu on 5/14/21.
//

#include "Mesh.hpp"

Mesh::Mesh() {
    
}

Mesh::Mesh(std::vector<AnyVertex*>& vertices_,
     std::vector<GLuint>& indices_,
           TextureMaps& textures_, VertexType vertexType_) :
VertexData(vertices_, indices_, textures_, vertexType_){

} 

