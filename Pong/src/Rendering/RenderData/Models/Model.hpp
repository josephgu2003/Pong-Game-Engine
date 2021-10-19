//
//  Model.hpp
//  Pong
//
//  Created by Joseph Gu on 5/14/21.
//

#ifndef Model_hpp
#define Model_hpp

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.hpp"
#include <vector>
#include <string>
#include "VertexData.hpp"
#include <map>
#include "Bone.hpp"

// is a model
#define DIFFUSE 0
#define SPECULAR 1

class AnimComponent;

struct Hitbox {
    float maxX, minX, maxY, minY, maxZ, minZ;
};

class Model  {
    friend class Billow;
private:
    std::map<std::string, BoneData> BoneDataMap;
    
    int boneCounter = 0;
    
    std::vector<Mesh> meshes;
    std::vector<Texture> loadedTextures;
    std::string directory;
    std::string path;

    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);
    GLuint getTextureFromFile(std::string path);

    void BoneWeightVertices(std::vector<TBNBWVertex*>& vertices, aiMesh* mesh,
                            const aiScene* scene);
    
    glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from);
    
    
    void setVertexBoneData(TBNBWVertex* v, int id, float weight);
public:
    Model();
    Model(const char* filePath);
    Model(const char* filePath, AnimComponent* anim);
    
    
    ~Model();
    
    void loadModel(std::string filePath);
    void loadModel(std::string filePath_, AnimComponent* anim_);
    
    void setMeshes(std::vector<Mesh> meshes_);
    void setMeshTexture(int index, TextureMaps& textures);
    void setDirectory(const char* directory_);
    std::string getDirectory();
    void setFilepath(const char* path_);
    std::string getFilepath();
    std::vector<Mesh>* getMeshes();
};
#endif /* Model_hpp */
