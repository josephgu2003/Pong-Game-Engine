//
//  VertexLoader.hpp
//  Pong
//
//  Created by Joseph Gu on 10/16/21.
//

#ifndef VertexLoader_hpp
#define VertexLoader_hpp

#include <stdio.h>
#include <map>
#include <assimp/scene.h>
#include <GL/glew.h>
#include "AnimComponent.hpp"

struct TextureMaps;


struct AnyVertex {};

enum VertexType {
    VERTEX_VERTEX, VERTEX_SIMPLEVERTEX, VERTEX_TBNVERTEX, VERTEX_TBNBWVERTEX
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


class VertexLoader {
private:
    static int indexOffset;
    static std::map<std::string, BoneInfoMap> loadedBoneDataMaps;
    static BoneInfoMap inProgBoneMap;
    static int boneCounter;
    
    static glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from);
    static void BoneWeightVertices(std::vector<TBNBWVertex>& vertices, aiMesh* mesh,
                            const aiScene* scene);
    static void processNode(aiNode* node, const aiScene* scene, std::vector<TBNBWVertex>& vertices, std::vector<GLuint>& indices);
    static void processMesh(aiMesh* mesh, const aiScene* scene, std::vector<TBNBWVertex>& vertices,  std::vector<GLuint>& indices);
    static void setVertexBoneData(TBNBWVertex* v, int id, float weight);
    static void reset();
public: 
   // static std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);
  //  static GLuint getTextureFromFile(std::string path);
    // ^^ what about making them work??
    
    static void loadModel(std::string filePath, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices);
    static void loadModelAnimations(AnimComponent* anim_, std::string filePath_);
    static void loadPoint(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices);
     
    static void loadTextData(const std::string& s, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, TextureMaps& map, glm::vec2 position);
    static void setupVAOAttribs(VertexType vt);
    static void setupVAOAttribsInstancing(int firstAttribLocation, const std::vector<int>& layout);
    static void load2DQuadData(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, glm::vec2 dimensions, glm::vec2 position);
    static void loadMapChunk(const std::string& src, int chunkX, int chunkY, int subdivisions, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices);

};
  
#endif /* VertexLoader_hpp */
