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


class Billow;

struct Vertex {
    glm::vec3 Pos;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct TBNVertex {
    glm::vec3 Pos;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tan;
    glm::vec3 BiTan;
};

struct SimpleVertex {
    glm::vec3 Pos;
    glm::vec2 TexCoords;
};

struct Texture {
    GLuint id;
    std::string type;
    std::string path;
};

class Mesh {
    friend class Billow;
public:
    std::vector<TBNVertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    
    Mesh();
    
    Mesh(std::vector<TBNVertex> vertices_,
         std::vector<GLuint> indices_,
         std::vector<Texture> textures_);
    
    void setVertexData(std::vector<TBNVertex> vertices_,
                 std::vector<GLuint> indices_,
                 std::vector<Texture> textures_);
};
#endif /* Mesh_hpp */
