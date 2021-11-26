//
//  Vertices.hpp
//  Pong
//
//  Created by Joseph Gu on 11/5/21.
//

#ifndef Vertices_hpp
#define Vertices_hpp

#include <stdio.h>
#include <glm/glm.hpp>
 
#define MAX_BONE_WEIGHTS 4
 
struct AnyVertex {};

enum VertexType {
    VERTEX_VERTEX, VERTEX_SIMPLEVERTEX, VERTEX_TBNVERTEX, VERTEX_TBNBWVERTEX
};

struct PosVertex : AnyVertex {
    glm::vec3 Pos;
    PosVertex() {
         
    }
    PosVertex(const glm::vec3& pos_) {
        Pos = pos_;
    }
};

struct Vertex : AnyVertex {
    glm::vec3 Pos;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    Vertex( glm::vec3 Pos_,
           glm::vec3 Normal_,
           glm::vec2 TexCoords_) {
        Pos = Pos_;
        Normal = Normal_;
        TexCoords = TexCoords_;
    }
    Vertex() {
        
    }
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
              glm::vec2 TexCoords, int arraytexID_);
    glm::vec3 Pos;
    glm::vec2 TexCoords;
    float arraytexID;
    SimpleVertex() {}
};


#endif /* Vertices_hpp */
