//
//  Model.cpp
//  Pong
//
//  Created by Joseph Gu on 5/14/21.
//

#include "Model.hpp"
#include "stb_image.h"
#include <iostream>
#include "AssetManager.hpp"

Model::Model() {
}
Model::Model(const char* filePath_) {
    loadModel(filePath_);
}

Model::~Model() {
}

void Model::loadModel(std::string filePath_) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath_,  aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
    
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString();
        return;
    }
    path = filePath_;
    directory = filePath_.substr(0, filePath_.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}
void Model::setMeshTexture(int index, TextureMaps& textures) {
            meshes.at(index).setTextures(textures);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    //load each mesh from node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector< std::shared_ptr<AnyVertex>> vertices;
    std::vector<GLuint> indices;
    TextureMaps textures;
    
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) { //iterate over mesh vertices
        
        glm::vec3 pos_;
        pos_.x = mesh->mVertices[i].x;
        pos_.y = mesh->mVertices[i].y;
        pos_.z = mesh->mVertices[i].z;
        
        glm::vec3 norm_;
        norm_.x = mesh->mNormals[i].x;
        norm_.y = mesh->mNormals[i].y;
        norm_.z = mesh->mNormals[i].z;
        
        glm::vec3 Tan_;
        Tan_.x = mesh->mTangents[i].x;
      Tan_.y = mesh->mTangents[i].y;
       Tan_.z = mesh->mTangents[i].z;
        
        glm::vec3 BiTan_;
       BiTan_.x = mesh->mBitangents[i].x;
       BiTan_.y = mesh->mBitangents[i].y;
       BiTan_.z = mesh->mBitangents[i].z;
        
        glm::vec2 texCoords_;
        if (mesh->mTextureCoords[0]) {
            texCoords_.x = mesh->mTextureCoords[0][i].x;
            texCoords_.y = mesh->mTextureCoords[0][i].y;
        } else {
            texCoords_ = glm::vec2(0.0f, 0.0f);
        }
        
        std::shared_ptr<TBNVertex> vertex = std::make_shared<TBNVertex>(pos_, norm_, texCoords_, Tan_, BiTan_);
        vertices.push_back(vertex);
    }
    
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) { // iterate over mesh faces
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        if (diffuseMaps.size() != 0)
        textures.diffuse = diffuseMaps.at(0); //???? lol who would have more than 1 texture
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        if (specularMaps.size() != 0)
        textures.specular = specularMaps.at(0);
    } 
    Mesh returnedMesh(vertices, indices, textures, VERTEX_TBNVERTEX);
    return returnedMesh;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type_, std::string typeName) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(type_); i++) {
        aiString string;
        
        material->GetTexture(type_, i, &string);
        bool texIsRepeat = false;
        std::string string_ = (std::string)string.C_Str();
        std::string fileName = string_.substr(string_.find_last_of("\\")+ 1);
        std::string filePath = directory+"/"+fileName;
        
        for (int j = 0; j < loadedTextures.size(); j++) {
            if (std::strcmp(loadedTextures[j].path.data(), filePath.c_str()) == 0) {
                textures.push_back(loadedTextures[j]);
                std::cout << loadedTextures[j].path.data() << "\n";
                texIsRepeat = true;
                break;
            }
        }
        if (!texIsRepeat) {
            Texture texture;
            bool srgb = false;
            if (type_ == aiTextureType_DIFFUSE) srgb = true;
            AssetManager::loadTexture(filePath.c_str(), &texture, srgb);
            texture.type = typeName;
            texture.path = filePath;
            textures.push_back(texture);
            loadedTextures.push_back(texture);
        }
    }
    return textures;
}




void Model::setMeshes(std::vector<Mesh> meshes_) {
    meshes = meshes_;
}
void Model::setDirectory(const char* directory_) {
    directory = directory_;
}

std::string Model::getDirectory() {
    return directory;
}

void Model::setFilepath(const char* path_) {
    path = path_;
}

std::string Model::getFilepath() {
    return path;
}

std::vector<Mesh>* Model::getMeshes() {
    return &meshes;
}

void Model::setHitbox(Hitbox hitbox_) {
    hitbox = hitbox_;
}
