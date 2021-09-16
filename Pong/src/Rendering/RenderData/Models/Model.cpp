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
#include "AnimComponent.hpp"

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
 
void Model::loadModel(std::string filePath_, AnimComponent* anim_) {
    Assimp::Importer importer;
     
    importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false); 
    const aiScene* scene = importer.ReadFile(filePath_,  aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
     
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString();
        return;      
    }
    path = filePath_;        
    directory = filePath_.substr(0, filePath_.find_last_of('/'));
    processNode(scene->mRootNode, scene);
    anim_->setBoneDataMap(BoneDataMap); 
    for (int i = 0; i < scene->mNumAnimations; i ++) {
        anim_->addAnimation(scene->mAnimations[i], scene);
    }

}

Model::Model(const char* filePath, AnimComponent* anim_) {
    loadModel(filePath, anim_);
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
    std::vector< std::shared_ptr<TBNBWVertex>> vertices;
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
        
        int boneIDs[MAX_BONE_WEIGHTS];
        float boneWeights[MAX_BONE_WEIGHTS];
        for (int i = 0; i < MAX_BONE_WEIGHTS; i++)
           {
               boneIDs[i] = -1;
               boneWeights[i] = 0.25f;
           }
        
        std::shared_ptr<TBNBWVertex> vertex = std::make_shared<TBNBWVertex>(pos_, norm_, texCoords_, Tan_, BiTan_, boneIDs, boneWeights);
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
    BoneWeightVertices(vertices, mesh, scene);
    std::vector<std::shared_ptr<AnyVertex>> newVertices;
    newVertices.resize(vertices.size());
    for (int i = 0; i < vertices.size(); i++) {
        newVertices[i] = vertices[i];
    }
    Mesh returnedMesh(newVertices, indices, textures, VERTEX_TBNVERTEX);
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

void Model::BoneWeightVertices(std::vector<std::shared_ptr<TBNBWVertex>>& vertices, aiMesh* mesh,
                               const aiScene* scene) { 
    printf("Here: %f \n", (float)mesh->mNumBones);
    int x = mesh->mNumBones;
  //  if (x > 4) x = 4;
    for (int boneIndex = 0; boneIndex < x; ++boneIndex) {
        int id = -1;
        std::string boneName(mesh->mBones[boneIndex]->mName.C_Str());
      
        std::cout << boneName << "\n" ; 
        if (BoneDataMap.find(boneName) == BoneDataMap.end())
        {
            BoneData data;
            data.id = boneCounter;
            data.offset =
                 ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
            if (data.offset == glm::mat4(1.0f)) {

            }
            BoneDataMap[boneName] = data;
            id = BoneDataMap.size()-1;
            boneCounter++;
        }
        else {
            id = BoneDataMap[boneName].id;
            if (BoneDataMap[boneName].offset !=   ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix)) {
                printf("Very error! \n");
            }  
        }
        // id of bone from mesh obtained,  or new bone added to map
        assert(id != -1);
        auto weights = mesh->mBones[boneIndex]->mWeights; //obtain vertex weights for this bone
        int numWeights = mesh->mBones[boneIndex]->mNumWeights; //obtain number of weights

        for (int i = 0; i < numWeights; i++) {
            int vertexId = weights[i].mVertexId;
                  float weight = weights[i].mWeight;
                  assert(vertexId <= vertices.size());
                setVertexBoneData(*vertices[vertexId].get(), id, weight);
        }
    }
    for (int i = 0; i < vertices.size(); i++) {
        float sum = 0.0f;
        for (int j =0 ; j < MAX_BONE_WEIGHTS; j++) {
            if (vertices.at(i)->boneIDs[j] != -1)
            sum +=  vertices.at(i)->boneWeights[j];   
        }
        float factor = 1.0f / sum;
        for (int j =0 ; j < MAX_BONE_WEIGHTS; j++) { 
            if (vertices.at(i)->boneIDs[j] != -1)
            vertices.at(i)->boneWeights[j] *= factor;
        }
     }
}
 
glm::mat4 Model::ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
{
    glm::mat4 to;
    //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
    to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
    to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
    to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
    to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
    for (int y = 0; y < 4; y++)
        { 
            for (int x = 0; x < 4; x++)
            {
                to[x][y] = from[y][x];
            }
        }
    return to;
}


void Model::setVertexBoneData(TBNBWVertex& v, int id, float weight) {
    for (int i = 0; i< MAX_BONE_WEIGHTS; i++) {
        if (v.boneIDs[i] == -1) {
            v.boneIDs[i] = id;
            v.boneWeights[i] = weight;
            return;
        }
    }
    printf("uh oh");
}

 
