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

// is a model
#define DIFFUSE 0
#define SPECULAR 1

struct Hitbox {
    float maxX, minX, maxY, minY, maxZ, minZ;
};

class Model {
    friend class Billow;
private:
    std::vector<Mesh> meshes;
    std::vector<Texture> loadedTextures;
    std::string directory;
    Hitbox hitbox;
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);
    GLuint getTextureFromFile(std::string path);
    void loadModel(std::string filePath);
public:
    Model();
    Model(const char* filePath);
    ~Model();
    void setMeshes(std::vector<Mesh> meshes_);
    void setMeshTexture(int index, int type, std::vector<GLuint> newDiffMaps, std::vector<GLuint> newSpecMaps);
    void setDirectory(const char* directory_);
    std::string getDirectory();
    std::vector<Mesh>* getMeshes();
    void setHitbox(Hitbox hitbox_);
};
#endif /* Model_hpp */
