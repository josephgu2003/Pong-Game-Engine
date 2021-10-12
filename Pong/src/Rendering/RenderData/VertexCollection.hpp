//
//  VertexCollection.hpp
//  Pong
//
//  Created by Joseph Gu on 10/10/21.
//

#ifndef VertexCollection_hpp
#define VertexCollection_hpp

/**
enum VertexType {
    VERTEX_VERTEX, VERTEX_SIMPLEVERTEX, VERTEX_TBNVERTEX
};

struct AnyVertex {};

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

struct TBNBWVertex : AnyVertex {
    TBNBWVertex();
    TBNBWVertex(    glm::vec3 Pos,
              glm::vec3 Normal,
              glm::vec2 TexCoords,
              glm::vec3 Tan,
              glm::vec3 BiTan, int* boneIDs, float* boneWeights);
    glm::vec3 Pos;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tan;
    glm::vec3 BiTan;
    int boneIDs[MAX_BONE_WEIGHTS];
    float boneWeights[MAX_BONE_WEIGHTS];
};

struct SimpleVertex : AnyVertex {
    SimpleVertex(    glm::vec3 Pos,
              glm::vec2 TexCoords);
    glm::vec3 Pos;
    glm::vec2 TexCoords;
};

enum VertexTemplate {
    QUAD_SIMPLE
};

class VertexCollection {
private:
    std::vector<float> vertices;
    std::vector<GLuint> indices;
    TextureMaps textures;
public:
    VertexCollection(VertexTemplate vt);
    VertexCollection();
};**/

#endif /* VertexCollection_hpp */
