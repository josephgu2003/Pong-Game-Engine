//
//  VertexLoader.hpp
//  Pong
//
//  Created by Joseph Gu on 10/16/21.
//

#ifndef VertexLoader_hpp
#define VertexLoader_hpp

#include <stdio.h>
#include "VertexData.hpp"
#include <map>
#include <assimp/scene.h>
#include <gl/glew.h>
#include "AnimComponent.hpp"

struct TextureMaps;

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
    
    
    static void loadTextData(const std::string& s, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, TextureMaps& map);
    static void setupVAOAttribs(VertexType vt);
    static void load2DQuadData(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, glm::vec2 dimensions, glm::vec2 position);

};

#endif /* VertexLoader_hpp */
