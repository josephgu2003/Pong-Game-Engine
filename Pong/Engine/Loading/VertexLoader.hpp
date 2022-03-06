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

struct VertexData {
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    unsigned int numIndices;
    VertexData(unsigned int vao_,
               unsigned int vbo_,
               unsigned int ebo_,
               unsigned int numIndices_) {
        vao = vao_;
        vbo = vbo_;
        ebo = ebo_;
        numIndices = numIndices_;
    }
};
class VertexLoader {
private:
    static int indexOffset;
    static std::map<std::string, BoneInfoMap> loadedBoneDataMaps;
    static BoneInfoMap inProgBoneMap;
    static int boneCounter;
    
    static std::map<std::string, VertexData> loadedVertexData;

     
    static glm::vec3 calcNormalWithHeights(float cH, float dH, float uH, float rH, float lH);
    static void BoneWeightVertices(std::vector<TBNBWVertex>& vertices, aiMesh* mesh,
                            const aiScene* scene);

    static void processMesh(aiMesh* mesh, const aiScene* scene, std::vector<TBNBWVertex>& vertices,  std::vector<GLuint>& indices);

    static void processMesh(aiMesh* mesh, const aiScene* scene, std::vector<SimpleVertex>& vertices,  std::vector<GLuint>& indices);
    static void setVertexBoneData(TBNBWVertex* v, int id, float weight);
    static void reset();
    static bool loadAlreadyLoadedData(const std::string& name, unsigned int& vao, unsigned int& vbo, unsigned int& ebo, unsigned int& numIndices);
    static void loadTextDataAbstraction(const std::string& s, float fontsize, float linespace, float maxlinelength, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, Material& map, glm::vec2 position, bool useMaxLineLength);
public:
    static void ConvertMatrixToGLMFormat(const aiMatrix4x4& from, glm::mat4& to);
    static void loadMaterialTextures(Material& mat, aiMaterial* material);
 //   static GLuint getTextureFromFile(std::string path);
    // ^^ what about making them work??
    static void loadSimpleCube(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices);
    static void loadModel(std::string filePath, unsigned int& vao, unsigned int& vbo, unsigned int& ebo, unsigned int& numIndices, bool& deleteDataOnDestruct);
    static void loadModelSimple(std::string filePath, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices);
    static void loadModelAnimations(AnimComponent* anim_, std::string filePath_);
    static void loadPoint(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices);
    static void loadTextData(const std::string& s, float fontsize, float linespace, float maxlinelength, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, Material& map, glm::vec2 position);
    static void loadTextData(const std::string& s, float fontsize, float linespace, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, Material& map, glm::vec2 position);
    static void setupVAOAttribs(VertexType vt);
    static void setupVAOAttribsInstancing(int firstAttribLocation, const std::vector<int>& layout);
    static void load2DQuadData(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, glm::vec2 dimensions, glm::vec2 position);
    static void loadMapChunk(float heightMesh[CHUNK_DIM_PXLS][CHUNK_DIM_PXLS], const unsigned short* heightMap, int imageWidth, int imageHeight, int chunkX, int chunkY, glm::vec2 originPos, glm::vec3 scaling, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices);
    static void loadSimpleVertexGrid(int verticesX, int verticesY, float scale, std::vector<PosVertex>& mesh, unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int& numIndices);
    static void incTanBitanForTriangle(TBNVertex& v1, TBNVertex& v2, TBNVertex& v3);
    static void normalizeTanBitan(std::vector<TBNVertex>& vertices);
    
    template<typename T>
    static void loadModelAbstraction(std::string filePath_, unsigned int& vao, unsigned int& vbo, unsigned int& ebo, unsigned int& numIndices, bool& deleteDataOnDestruct, bool useLoadedData) {
        if (useLoadedData) {
            if (loadAlreadyLoadedData(filePath_, vao, vbo, ebo, numIndices)) {
                deleteDataOnDestruct = false;
                return;
            }
        }
        Assimp::Importer importer;
        importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
        const aiScene* scene = importer.ReadFile(filePath_,  aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
        
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::string s = "ERROR::ASSIMP::" + std::string(importer.GetErrorString()) + "\n";
            printf("%s", s.c_str());
            return;
        }
        
        std::vector<T> vertices;
        std::vector<GLuint> indices;
        
        processNode<T>(scene->mRootNode, scene, vertices, indices);
        
        fillVertexData<T>(vao, vbo, ebo, numIndices, GL_STATIC_DRAW, GL_STATIC_DRAW, vertices, indices);
        loadedVertexData.emplace(filePath_, VertexData(vao, vbo, ebo, numIndices));

        if (loadedBoneDataMaps.find(filePath_) == loadedBoneDataMaps.end()) {
            loadedBoneDataMaps.insert(std::pair<std::string, BoneInfoMap>(filePath_, inProgBoneMap));
        } 
        
        reset(); //might have other erferences to this
        deleteDataOnDestruct = false;
    }
    
    template<typename T>
    static void processNode(aiNode* node, const aiScene* scene, std::vector<T>& vertices,  std::vector<GLuint>& indices) {
        //load each mesh from node
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, scene, vertices, indices); // should be figured out by function overloading
        }
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode<T>(node->mChildren[i], scene, vertices, indices);
        }
    }
    
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
