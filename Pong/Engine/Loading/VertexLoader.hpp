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
#include "MapManager.hpp"
#include "Vertices.hpp"
#include "AnimComponent.hpp"

struct Material; 
class MeshComponent; 
class VertexLoader {
private:
    static glm::vec3 calcNormalWithHeights(float cH, float dH, float uH, float rH, float lH);
    static int indexOffset;
    static std::map<std::string, BoneInfoMap> loadedBoneDataMaps;
    static BoneInfoMap inProgBoneMap;
    static int boneCounter;
     
    static glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from);
    static void BoneWeightVertices(std::vector<TBNBWVertex>& vertices, aiMesh* mesh,
                            const aiScene* scene);
    static void processNode(aiNode* node, const aiScene* scene, std::vector<TBNBWVertex>& vertices, std::vector<GLuint>& indices);
    static void processMesh(aiMesh* mesh, const aiScene* scene, std::vector<TBNBWVertex>& vertices,  std::vector<GLuint>& indices);
    static void processNode(aiNode* node, const aiScene* scene, std::vector<SimpleVertex>& vertices, std::vector<GLuint>& indices);
    static void processMesh(aiMesh* mesh, const aiScene* scene, std::vector<SimpleVertex>& vertices,  std::vector<GLuint>& indices);
    static void setVertexBoneData(TBNBWVertex* v, int id, float weight);
    static void reset();
public: 
    static std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName); 
 //   static GLuint getTextureFromFile(std::string path);
    // ^^ what about making them work??
    
    static void loadModel(std::string filePath, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices);
    static void loadModelSimple(std::string filePath, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices);
    static void loadModelAnimations(AnimComponent* anim_, std::string filePath_);
    static void loadPoint(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices);
     
    static void loadTextData(const std::string& s, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, Material& map, glm::vec2 position);
    static void setupVAOAttribs(VertexType vt);
    static void setupVAOAttribsInstancing(int firstAttribLocation, const std::vector<int>& layout);
    static void load2DQuadData(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, glm::vec2 dimensions, glm::vec2 position);
    static void loadMapChunk(float heightMesh[CHUNK_DIM_PXLS][CHUNK_DIM_PXLS], const unsigned short* heightMap, int imageWidth, int imageHeight, int chunkX, int chunkY, glm::vec2 originPos, glm::vec3 scaling, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices); 
    
    static void loadSimpleVertexGrid(int verticesX, int verticesY, float scale, std::vector<PosVertex>& mesh, unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int& numIndices);
    static void incTanBitanForTriangle(TBNVertex& v1, TBNVertex& v2, TBNVertex& v3);
    static void normalizeTanBitan(std::vector<TBNVertex>& vertices);
    template <typename T> 
    static void fillVertexData(unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int& numIndices, GLenum vboDrawMode, GLenum eboDrawMode, std::vector<T>& vertices, std::vector<GLuint>& indices) {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
            
        glBufferData(GL_ARRAY_BUFFER, sizeof(T)*vertices.size(), vertices.data(), vboDrawMode);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), indices.data(), eboDrawMode);
        
        numIndices = indices.size();
        VertexType vType = VERTEX_SIMPLEVERTEX;

        if (typeid(T) == typeid(SimpleVertex)) {
            vType = VERTEX_SIMPLEVERTEX;
        }
        if (typeid(T) == typeid(TBNVertex)) {
            vType = VERTEX_TBNVERTEX;
        }
        if (typeid(T) == typeid(TBNBWVertex)) {
            vType = VERTEX_TBNBWVERTEX;
        }
        if (typeid(T) == typeid(Vertex)) {
            vType = VERTEX_VERTEX;
        }
 
        setupVAOAttribs(vType);
        glBindVertexArray(0);
    }
}; 
  
#endif /* VertexLoader_hpp */
