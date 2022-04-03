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
class MultiMeshGraphicsComponent;

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
    template <typename T>
    struct IntermediateMesh {
        Material mat;
        std::vector<T> vertices;
        std::vector<GLuint> indices;
        int indexOffset;
    };
    static int indexOffset;
    static std::map<std::string, BoneInfoMap> loadedBoneDataMaps;
    static BoneInfoMap inProgBoneMap;
    static int boneCounter;
    
    static std::map<std::string, VertexData> loadedVertexData;
     
    static glm::vec3 calcNormalWithHeights(float cH, float dH, float uH, float rH, float lH);
    static void BoneWeightVertices(std::vector<TBNBWVertex>& vertices, aiMesh* mesh,
                            const aiScene* scene);
    
    static void processMesh(aiMesh* mesh, const aiScene* scene, std::vector<IntermediateMesh<TBNBWVertex>>& meshes, bool doMultiMeshing);

    static void processMesh(aiMesh* mesh, const aiScene* scene, std::vector<IntermediateMesh<SimpleVertex>>& meshes, bool doMultiMeshing);
    
    static void processMesh(aiMesh* mesh, const aiScene* scene,std::vector<IntermediateMesh<TBNVertex>>& meshes, bool doMultiMeshing);
    
    static void setVertexBoneData(TBNBWVertex* v, int id, float weight);
    static void reset();
    static bool loadAlreadyLoadedData(const std::string& name, unsigned int& vao, unsigned int& vbo, unsigned int& ebo, unsigned int& numIndices);
    static void loadTextDataAbstraction(const std::string& s, float fontsize, float linespace, float maxlinelength, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, Material& map, glm::vec2 position, bool useMaxLineLength);
    
    typename<typename T>
    void VertexLoader::processMeshAbstraction(aiMesh* mesh, const aiScene* scene, std::vector<IntermediateMesh<T>>& meshes, bool doMultiMeshing) {
        
        IntermediateMesh<T>* target = nullptr;
        
        if (meshes.size() == 0) {
            meshes.push_back(IntermediateMesh<T>());
        }
        
        target = &meshes.at(0);
        
        if (doMultiMeshing) {
            Material m;
            if (mesh->mMaterialIndex >= 0) {
                aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
                loadMaterialTextures(m, material);
            }
            for (auto& intermediateMesh : meshes) {
                if (intermediateMesh.mat == m) {
                    target = &intermediateMesh;
                    break;
                }
            }
        }
        
        assert(target!=NULL);
        
        std::vector<T>& vertices = target->vertices;
        std::vector<GLuint>& indices = target->indices;
        
        int firstIndex = vertices.size();
        
        vertices.reserve(vertices.size() + mesh->mNumVertices);
        
        for(unsigned int i = 0; i < mesh->mNumVertices; i++) { //iterate over mesh vertices
            addVertex<T>(vertices, i, mesh);
        }
        
        indices.reserve(indices.size() + mesh->mNumFaces);
        int offset = target->indexOffset;
        for(unsigned int i = 0; i < mesh->mNumFaces; i++) { // iterate over mesh faces
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                GLuint ind = face.mIndices[j] + offset;
                indices.push_back(ind);
            }
        }
        
        postProcessMesh(vertices, mesh, firstIndex);
        target->indexOffset += mesh->mNumVertices;
    }

    
public:
    static void ConvertMatrixToGLMFormat(const aiMatrix4x4& from, glm::mat4& to);
    static void loadMaterialTextures(Material& mat, aiMaterial* material);
 //   static GLuint getTextureFromFile(std::string path);
    // ^^ what about making them work??
    static void loadSimpleCube(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices);
    static void loadModel(std::string filePath, unsigned int& vao, unsigned int& vbo, unsigned int& ebo, unsigned int& numIndices, bool& deleteDataOnDestruct);
    static void loadModelSimple(std::string filePath, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices);
    static void loadModelMultiMat(const std::string filePath, GraphicsObject& go);
    static void loadMultiMeshModel(const std::string filePath, MultiMeshGraphicsComponent* gc);
    static void loadModelAnimations(AnimComponent* anim_, std::string filePath_);
    static void loadPoint(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices);
    static void loadTextData(const std::string& s, float fontsize, float linespace, float maxlinelength, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, Material& map, glm::vec2 position);
    static void loadTextData(const std::string& s, float fontsize, float linespace, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, Material& map, glm::vec2 position);
 
    static void setupVAOAttribsInstancing(int firstAttribLocation, const std::vector<int>& layout);
    static void load2DQuadData(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, glm::vec2 dimensions, glm::vec2 position);
    static void loadMapChunk(float heightMesh[CHUNK_DIM_PXLS][CHUNK_DIM_PXLS], const unsigned short* heightMap, int imageWidth, int imageHeight, int chunkX, int chunkY, glm::vec2 originPos, glm::vec3 scaling, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices);
    static void loadSimpleVertexGrid(int verticesX, int verticesY, float scale, std::vector<PosVertex>& mesh, unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int& numIndices);
    static void loadSimpleVertexGrid(int verticesX, int verticesY, float scale, glm::vec2 offset, unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int& numIndices);
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
        
        std::vector<IntermediateMesh<SimpleVertex>> meshes;
        
        processNode<T>(scene->mRootNode, scene, meshes);
        
        fillVertexData<T>(vao, vbo, ebo, numIndices, GL_STATIC_DRAW, GL_STATIC_DRAW, meshes.at(0).vertices, meshes.at(0).indices);
        loadedVertexData.emplace(filePath_, VertexData(vao, vbo, ebo, numIndices));

        if (loadedBoneDataMaps.find(filePath_) == loadedBoneDataMaps.end()) {
            loadedBoneDataMaps.insert(std::pair<std::string, BoneInfoMap>(filePath_, inProgBoneMap));
        } 
        
        reset(); //might have other erferences to this
        deleteDataOnDestruct = false;
    }
    
    template<typename T>
    static void processNode(aiNode* node, const aiScene* scene, std::vector<IntermediateMesh<SimpleVertex>>& meshes, bool doMultiMeshing) {
        //load each mesh from node
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            processMeshAbstraction<T>(mesh, scene, meshes, doMultiMeshing); // should be figured out by function overloading
        }
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode<T>(node->mChildren[i], scene, meshes, doMultiMeshing);
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
        
        setupVAOAttribs<T>();
        glBindVertexArray(0);
    }
    
    template <typename T>
    static void setupVAOAttribs() {
        
    }
    
    
    static void setupVAOAttribsFromLayout (int vertexSize, std::vector<int>& numFloats, std::vector<int>& offsets) {
        if (numFloats.size() == offsets.size()) {
            for (int i = 0; i < numFloats.size(); i++) {
                glVertexAttribPointer(i, numFloats.at(i), GL_FLOAT, GL_FALSE,vertexSize, (void*)(offsets.at(i)));
                glEnableVertexAttribArray(i);
            }
        }
    }

};

template <>
inline void VertexLoader::setupVAOAttribs<SimpleVertex>() {
    std::vector<int> numFloats = {3,2,1};
    std::vector<int> offsets = {0, (sizeof(glm::vec3)),(sizeof(glm::vec2) + sizeof(glm::vec3))};
    setupVAOAttribsFromLayout(sizeof(SimpleVertex), numFloats, offsets);
}
 
template <>
inline void VertexLoader::setupVAOAttribs<Vertex>() {
    std::vector<int> numFloats = {3,3,2};
    std::vector<int> offsets = {0, (sizeof(glm::vec3)),(2*sizeof(glm::vec3))};
    setupVAOAttribsFromLayout(sizeof(Vertex), numFloats, offsets);
}

template <>
inline void VertexLoader::setupVAOAttribs<TBNVertex>() {
    std::vector<int> numFloats = {3,3,2,3,3};
    std::vector<int> offsets = {0, (sizeof(glm::vec3)),(2*sizeof(glm::vec3)), (2*sizeof(glm::vec3)+sizeof(glm::vec2)),(3*sizeof(glm::vec3)+sizeof(glm::vec2))};
    setupVAOAttribsFromLayout(sizeof(TBNVertex), numFloats, offsets);
}

template <>
inline void VertexLoader::setupVAOAttribs<TBNMVertex>() {
    std::vector<int> numFloats = {3,3,2,3,3};
    std::vector<int> offsets = {0, (sizeof(glm::vec3)),(2*sizeof(glm::vec3)), (2*sizeof(glm::vec3)+sizeof(glm::vec2)),(3*sizeof(glm::vec3)+sizeof(glm::vec2))};
    setupVAOAttribsFromLayout(sizeof(TBNVertex), numFloats, offsets);
    glVertexAttribIPointer(5, 1, GL_INT, sizeof(TBNBWVertex), (void*)(4*sizeof(glm::vec3)+sizeof(glm::vec2)));
    glEnableVertexAttribArray(5);
}

template <>
inline void VertexLoader::setupVAOAttribs<TBNBWVertex>() {
    std::vector<int> numFloats = {3,3,2,3,3};
    std::vector<int> offsets = {0, (sizeof(glm::vec3)),(2*sizeof(glm::vec3)), (2*sizeof(glm::vec3)+sizeof(glm::vec2)),(3*sizeof(glm::vec3)+sizeof(glm::vec2))};
    setupVAOAttribsFromLayout(sizeof(TBNBWVertex), numFloats, offsets);
    
    glVertexAttribIPointer(5, MAX_BONE_WEIGHTS, GL_INT, sizeof(TBNBWVertex), (void*)(4*sizeof(glm::vec3)+sizeof(glm::vec2)));
    glEnableVertexAttribArray(5);
    
    glVertexAttribPointer(6, MAX_BONE_WEIGHTS, GL_FLOAT, GL_FALSE, sizeof(TBNBWVertex), (void*)(MAX_BONE_WEIGHTS*sizeof(int)+4*sizeof(glm::vec3)+sizeof(glm::vec2)));
    glEnableVertexAttribArray(6);
}
  
#endif /* VertexLoader_hpp */
