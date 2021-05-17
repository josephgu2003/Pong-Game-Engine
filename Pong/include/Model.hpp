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
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include "stb_image.h"

class Model {
private:
    std::vector<Mesh> meshes;
    std::vector<Texture> loadedTextures;
    std::string directory;
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);
    GLuint getTextureFromFile(std::string path);
    void loadModel(std::string filePath);
public:
    Model();
    Model(const char* filePath);
    ~Model();
    std::vector<Mesh>* getMeshes();
};
#endif /* Model_hpp */
