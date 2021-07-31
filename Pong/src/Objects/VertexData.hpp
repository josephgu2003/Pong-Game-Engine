//
//  VertexData.hpp
//  Pong
//
//  Created by Joseph Gu on 7/14/21.
//

#ifndef VertexData_hpp
#define VertexData_hpp

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "Component.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <string>

struct AnyVertex {};

enum VertexType {
    VERTEX_VERTEX, VERTEX_SIMPLEVERTEX, VERTEX_TBNVERTEX
};

struct Vertex : AnyVertex {
    glm::vec3 Pos;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct TBNVertex : AnyVertex {
    TBNVertex();
    TBNVertex(    glm::vec3 Pos,
              glm::vec3 Normal,
              glm::vec2 TexCoords,
              glm::vec3 Tan,
              glm::vec3 BiTan);
    glm::vec3 Pos;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tan;
    glm::vec3 BiTan;
};

struct SimpleVertex : AnyVertex {
    SimpleVertex(    glm::vec3 Pos,
              glm::vec2 TexCoords);
    glm::vec3 Pos;
    glm::vec2 TexCoords;
};

struct Texture {
    GLuint id;
    std::string type;
    std::string path;
    glm::vec3 dimensions;
    GLenum textureTarget;
    
};

struct TextureMaps {
    Texture diffuse;
    Texture specular;
    Texture normMap;
    Texture noise;
    Texture gradient;
    Texture voronoi;
};

class VertexData {
protected:
    std::vector<std::shared_ptr<AnyVertex>> vertices;
    std::vector<GLuint> indices;
    TextureMaps textures;
    VertexType vertexType;
public:
    VertexData();

    VertexData(std::vector<std::shared_ptr<AnyVertex>>& vertices_,
         std::vector<GLuint>& indices_,
               TextureMaps& textures_, VertexType vertexType_);
    void setVertexData(std::vector<std::shared_ptr<AnyVertex>>& vertices_,
                 std::vector<GLuint>& indices_,
                       TextureMaps& textures_, VertexType vertexType_);

    std::vector<std::shared_ptr<AnyVertex>>& getVertices();

    void setVertices(std::vector<std::shared_ptr<AnyVertex>>& vertices_, VertexType vertexType_);
    
    std::vector<GLuint>& getIndices();
    void setIndices(std::vector<GLuint>& indices_);
    
    TextureMaps& getTextures();
    void setTextures(TextureMaps& textures_);
};

#endif /* VertexData_hpp */
