//
//  Mesh.hpp
//  Pong
//
//  Created by Joseph Gu on 5/14/21.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include <string>
#include <vector>
#include "VertexData.hpp"

class Billow;

class Mesh : public VertexData {
    friend class Billow;
public:
    Mesh();
    Mesh(std::vector<std::shared_ptr<AnyVertex>>& vertices_,
         std::vector<GLuint>& indices_,
                TextureMaps& textures_, VertexType vertexType_);
};
#endif /* Mesh_hpp */
