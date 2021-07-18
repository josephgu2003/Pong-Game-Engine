//
//  Model.cpp
//  Pong
//
//  Created by Joseph Gu on 5/14/21.
//

#include "Model.hpp"
#include "stb_image.h"
#include <iostream>

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
    
    directory = filePath_.substr(0, filePath_.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}
void Model::setMeshTexture(int index, int type, std::vector<GLuint> newDiffMaps, std::vector<GLuint> newSpecMaps) {

        std::vector<Texture> newTextures;
        for (int i = 0 ; i<newDiffMaps.size();i++) {
            Texture diffTexture = {newDiffMaps.at(i), "test","test"};
            newTextures.push_back(diffTexture);
            Texture specTexture = {newSpecMaps.at(i), "test","test"};
            newTextures.push_back(specTexture);

            meshes.at(index).setTextures(newTextures);
    }
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    //load each mesh from node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        std::cout << meshes.size() << "\n";
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
    std::vector<Texture> textures;
    
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
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
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
            texture.id = getTextureFromFile(filePath);
            texture.type = typeName;
            texture.path = filePath;
            textures.push_back(texture);
            loadedTextures.push_back(texture);
        }
    }
    return textures;
}

GLuint Model::getTextureFromFile(std::string texturePath_) {
    GLuint tex = 0;
    int imageWidth = 0;
    int imageHeight = 0;
    int channels = 0;
    unsigned char* imageData;
    std::cout << texturePath_ << "\n";
    const char* texturePath = texturePath_.c_str();
    
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    
    imageData = stbi_load(texturePath, &imageWidth, &imageHeight, &channels, 0);
    if (imageData) {
        GLenum format;
               if (channels == 1)
                   format = GL_RED;
               else if (channels == 3)
                   format = GL_RGB;
               else if (channels == 4)
                   format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture data \n" << stbi_failure_reason() << "\n";
    }
    stbi_image_free(imageData);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
   // glBindTexture(GL_TEXTURE_2D, 0);
    return tex;
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

std::vector<Mesh>* Model::getMeshes() {
    return &meshes;
}

void Model::setHitbox(Hitbox hitbox_) {
    hitbox = hitbox_;
}
