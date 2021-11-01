//
//  VertexLoader.cpp
//  Pong
//
//  Created by Joseph Gu on 10/16/21.
//

#include "VertexLoader.hpp" 
#include <GL/glew.h> 
#define GLFW_DLL 
#include <GLFW/glfw3.h>  
#include "AssetManager.hpp" 
#include <glm/glm.hpp>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include "stb_image.h"


TBNBWVertex::TBNBWVertex() {
    
}
TBNBWVertex::TBNBWVertex(    glm::vec3 Pos_,
          glm::vec3 Normal_,
          glm::vec2 TexCoords_,
          glm::vec3 Tan_,
                   glm::vec3 BiTan_, int* boneIDs_, float* boneWeights_) {
    Pos = Pos_;
    Normal = Normal_;
    TexCoords = TexCoords_;
    Tan = Tan_;
    BiTan = BiTan_;
    for (int i = 0; i < MAX_BONE_WEIGHTS; i++) {
        boneIDs[i] = boneIDs_[i];
        boneWeights[i] = boneWeights_[i];
    }
}

TBNVertex::TBNVertex() {
    
}

TBNVertex::TBNVertex(    glm::vec3 Pos_,
          glm::vec3 Normal_,
          glm::vec2 TexCoords_,
          glm::vec3 Tan_,
          glm::vec3 BiTan_) {
    Pos = Pos_;
    Normal = Normal_;
    TexCoords = TexCoords_;
    Tan = Tan_;
    BiTan = BiTan_;
}

SimpleVertex::SimpleVertex(    glm::vec3 Pos_,
                           glm::vec2 TexCoords_, int arraytexID_) {
    Pos = Pos_;
    TexCoords = TexCoords_;
    arraytexID = arraytexID_;
}


BoneInfoMap VertexLoader::inProgBoneMap;
int VertexLoader::boneCounter = 0; 
std::map<std::string, BoneInfoMap> VertexLoader::loadedBoneDataMaps;
int VertexLoader::indexOffset = 0;

void VertexLoader::loadTextData(const std::string& text, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, TextureMaps& map, glm::vec2 position) {
    glBindVertexArray(vao); 
    
    std::map<char, Character> characters; 

    AssetManager::loadGlyphs("Resources/Glyphs/times.ttf", characters, map);

    std::string::const_iterator c;
    float x = position.x;
    float y = position.y;
    float scale = 0.0002;  
        
    std::vector<SimpleVertex> newVertices;
    std::vector<GLuint> newIndices;
        
    int indexOffset = 0;
        
    for (c = text.begin(); c != text.end(); c++)
        {
            if (*c == '\n') {  
                x = position.x;
                y -= 0.07;
                continue; 
            }
            Character ch = characters[*c];

            float xpos = x + ch.bearing.x * scale;
            float ypos = y - (ch.size.y - ch.bearing.y) * scale;

            float w = ch.size.x * scale;
             
            float h = ch.size.y * scale;
              
            float texX, texY = 0;
              
            texX = ch.texCoords.x;
            texY = ch.texCoords.y;
              
            int id = ch.id;
               
            newVertices.emplace_back(glm::vec3(xpos, ypos + h, 0), glm::vec2(0.0f, 0.0f), id);
            newVertices.emplace_back(glm::vec3(xpos, ypos, 0),  glm::vec2(0.0f, texY), id);
            newVertices.emplace_back(glm::vec3(xpos + w, ypos, 0),  glm::vec2(texX, texY), id);
            newVertices.emplace_back(glm::vec3(xpos + w, ypos + h, 0),  glm::vec2(texX, 0.0f), id);
              
            std::vector<GLuint> newIndices2 = {
                  1, 0, 3, 1, 2, 3
              };

              for (int i = 0; i < newIndices2.size(); i++) {
                  GLuint ind = newIndices2.at(i) + indexOffset;
                  newIndices.push_back(ind);
              }
              
              x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
              indexOffset += 4;
          }
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, newVertices.size() * sizeof(SimpleVertex), newVertices.data(), GL_STATIC_DRAW);
    
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, newIndices.size() * sizeof(GLuint), newIndices.data(), GL_STATIC_DRAW);
    
    numIndices = newIndices.size();
    setupVAOAttribs(VERTEX_SIMPLEVERTEX);
     
    glBindVertexArray(0);
}

void VertexLoader::setupVAOAttribs(VertexType vt) {
    switch (vt) {
        case VERTEX_SIMPLEVERTEX:
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,sizeof(SimpleVertex), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,  sizeof(SimpleVertex), (void*)(sizeof(glm::vec3)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE,  sizeof(SimpleVertex), (void*)(sizeof(glm::vec2) + sizeof(glm::vec3)));
            glEnableVertexAttribArray(2);
            break;
            
        case VERTEX_VERTEX:
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)(sizeof(glm::vec3)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)(2*sizeof(glm::vec3)));
            glEnableVertexAttribArray(2);
            break;
            
        case VERTEX_TBNVERTEX:
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)0);
            glEnableVertexAttribArray(0);
            
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)(sizeof(glm::vec3)));
            glEnableVertexAttribArray(1);
            
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)(2*sizeof(glm::vec3)));
            glEnableVertexAttribArray(2);
            
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)(2*sizeof(glm::vec3)+sizeof(glm::vec2)));
            glEnableVertexAttribArray(3);
            
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)(3*sizeof(glm::vec3)+sizeof(glm::vec2)));
            glEnableVertexAttribArray(4);
            break;
            
        case VERTEX_TBNBWVERTEX:
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TBNBWVertex), (void*)0);
            glEnableVertexAttribArray(0);
            
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TBNBWVertex), (void*)(sizeof(glm::vec3)));
            glEnableVertexAttribArray(1);
            
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TBNBWVertex), (void*)(2*sizeof(glm::vec3)));
            glEnableVertexAttribArray(2);
            
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TBNBWVertex), (void*)(2*sizeof(glm::vec3)+sizeof(glm::vec2)));
            glEnableVertexAttribArray(3);
            
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(TBNBWVertex), (void*)(3*sizeof(glm::vec3)+sizeof(glm::vec2)));
            glEnableVertexAttribArray(4);
            
            glVertexAttribIPointer(5, MAX_BONE_WEIGHTS, GL_INT, sizeof(TBNBWVertex), (void*)(4*sizeof(glm::vec3)+sizeof(glm::vec2)));
            glEnableVertexAttribArray(5);
            
            glVertexAttribPointer(6, MAX_BONE_WEIGHTS, GL_FLOAT, GL_FALSE, sizeof(TBNBWVertex), (void*)(MAX_BONE_WEIGHTS*sizeof(int)+4*sizeof(glm::vec3)+sizeof(glm::vec2)));
            glEnableVertexAttribArray(6);
            
            break;
            
    }
}

void VertexLoader::load2DQuadData(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, glm::vec2 dimensions, glm::vec2 position) {
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    std::vector<SimpleVertex> newVertices = {SimpleVertex(glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0), 0.0), SimpleVertex(glm::vec3(0.0, 1.0, 0.0), glm::vec2(0.0, 1.0), 0.0), SimpleVertex(glm::vec3(1.0, 0.0, 0.0), glm::vec2(1.0, 0.0), 0.0), SimpleVertex(glm::vec3(1.0, 1.0, 0.0), glm::vec2(1.0, 1.0), 0.0)};
    for (int i = 0; i < newVertices.size(); i++) {
        SimpleVertex& sv = newVertices.at(i);
        sv.Pos.x *= dimensions.x;
        sv.Pos.y *= dimensions.y;
        sv.Pos += glm::vec3(position.x, position.y, 0.0f);
    }
    
    glBufferData(GL_ARRAY_BUFFER, newVertices.size() * sizeof(SimpleVertex), newVertices.data(), GL_STATIC_DRAW);
    
    std::vector<GLuint> newIndices2 = {
          0, 1, 3, 0, 2, 3
    };
    
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, newIndices2.size() * sizeof(GLuint), newIndices2.data(), GL_STATIC_DRAW);
    
    numIndices = newIndices2.size();
    setupVAOAttribs(VERTEX_SIMPLEVERTEX);
    glBindVertexArray(0);
}

void VertexLoader::loadModel(std::string filePath_, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices) {
        Assimp::Importer importer;
    importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    const aiScene* scene = importer.ReadFile(filePath_,  aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
       
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::string s = "ERROR::ASSIMP::" + std::string(importer.GetErrorString()) + "\n";
            printf("%s", s.c_str());
            return;
        }
 
    std::vector<TBNBWVertex> vertices;
    std::vector<GLuint> indices; 
    processNode(scene->mRootNode, scene, vertices, indices);
    
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TBNBWVertex), vertices.data(), GL_STATIC_DRAW);
    
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    
    numIndices = indices.size(); 
    setupVAOAttribs(VERTEX_TBNBWVERTEX);
    glBindVertexArray(0);
    
 if (loadedBoneDataMaps.find(filePath_) == loadedBoneDataMaps.end()) {
     loadedBoneDataMaps.insert(std::pair<std::string, BoneInfoMap>(filePath_, inProgBoneMap));
 }
    reset(); //might have other erferences to this
}  
 
void VertexLoader::loadModelAnimations(AnimComponent* anim_, std::string filePath_) {
    
    Assimp::Importer importer;
    importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    
    const aiScene* scene = importer.ReadFile(filePath_,  aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
       
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::string s = "ERROR::ASSIMP::" + std::string(importer.GetErrorString()) + "\n";
            printf("%s", s.c_str());
            return;
        }
   
    if (loadedBoneDataMaps.find(filePath_) == loadedBoneDataMaps.end()) {
        std::vector<TBNBWVertex> vertices;
        std::vector<GLuint> indices;
        processNode(scene->mRootNode, scene, vertices, indices);
        loadedBoneDataMaps.insert(std::pair<std::string, BoneInfoMap>(filePath_, inProgBoneMap));
    }
    BoneInfoMap& bim = loadedBoneDataMaps.find(filePath_)->second;
    anim_->setBoneDataMap(bim);
    for (int i = 0; i < scene->mNumAnimations; i ++) {
        anim_->addAnimation(scene->mAnimations[i], scene);
    }
    reset();
}


void VertexLoader::processNode(aiNode* node, const aiScene* scene, std::vector<TBNBWVertex>& vertices,  std::vector<GLuint>& indices) {
    //load each mesh from node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene, vertices, indices);
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, vertices, indices);
    }
}

void VertexLoader::processMesh(aiMesh* mesh, const aiScene* scene, std::vector<TBNBWVertex>& vertices, std::vector<GLuint>& indices) {
    
    std::vector<TBNBWVertex> newVertices;
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
        
       
        TBNBWVertex v(pos_, norm_, texCoords_, Tan_, BiTan_, boneIDs, boneWeights);
        newVertices.push_back(v);
    }
    
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) { // iterate over mesh faces
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            GLuint ind = face.mIndices[j] + indexOffset;
            indices.push_back(ind);
        }
    }
     
   /** if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        if (diffuseMaps.size() != 0)
        textures.diffuse = diffuseMaps.at(0); //???? lol who would have more than 1 texture
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        if (specularMaps.size() != 0)
        textures.specular = specularMaps.at(0);
    }
    **/
    BoneWeightVertices(newVertices, mesh, scene);
    vertices.insert(vertices.end(), newVertices.begin(), newVertices.end());
    indexOffset += mesh->mNumVertices;
}

/**std::vector<Texture> VertexLoader::loadMaterialTextures(aiMaterial* material, aiTextureType type_, std::string typeName) {
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
}**/

void VertexLoader::BoneWeightVertices(std::vector<TBNBWVertex>& vertices, aiMesh* mesh,
                               const aiScene* scene) {
    printf("Here: %f \n", (float)mesh->mNumBones);
    int x = mesh->mNumBones;

    for (int boneIndex = 0; boneIndex < x; ++boneIndex) {
        int id = -1;
        std::string boneName(mesh->mBones[boneIndex]->mName.C_Str());
      
        if (inProgBoneMap.find(boneName) == inProgBoneMap.end())
        {
            BoneData data;
            data.id = boneCounter;
            data.offset =
                 ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
            inProgBoneMap[boneName] = data;
            id = inProgBoneMap.size()-1;
            boneCounter++;
        }
        else {
            id = inProgBoneMap[boneName].id;
            if (inProgBoneMap[boneName].offset !=   ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix)) {
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
            setVertexBoneData(&vertices[vertexId], id, weight);
        }
    }
   /** for (int i = 0; i < vertices.size(); i++) {
        float sum = 0.0f;
        for (int j =0 ; j < MAX_BONE_WEIGHTS; j++) {
            if (vertices.at(i)->boneIDs[j] != -1)
            sum += vertices.at(i)->boneWeights[j];
        }
        float factor = 1.0f / sum;
        for (int j =0 ; j < MAX_BONE_WEIGHTS; j++) {
            if (vertices.at(i)->boneIDs[j] != -1)
            vertices.at(i)->boneWeights[j] *= factor;
        }
     }**/
}
 
glm::mat4 VertexLoader::ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
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


void VertexLoader::setVertexBoneData(TBNBWVertex* v, int id, float weight) {
    for (int i = 0; i< MAX_BONE_WEIGHTS; i++) {
        if (v->boneIDs[i] == -1) {
            v->boneIDs[i] = id;
            v->boneWeights[i] = weight;
            return;
        }
    }
}

void VertexLoader::reset() {

    inProgBoneMap.clear();
    indexOffset = 0;
    boneCounter = 0; //might have other erferences to this
}
  
void VertexLoader::setupVAOAttribsInstancing(int firstAttribLocation, const std::vector<int>& layout) { // assumes floats
    int byteCounter = 0;
    int bytesPerInstance = 0;
    for (int i = 0; i < layout.size(); i++) {
        bytesPerInstance += sizeof(float)*layout.at(i);
    }
    
    for (int i = 0; i < layout.size(); i++) {
        int attrib = i+firstAttribLocation;
        glVertexAttribPointer(attrib, layout.at(i), GL_FLOAT, GL_FALSE, bytesPerInstance, (void*)(byteCounter));
        glEnableVertexAttribArray(attrib);
        glVertexAttribDivisor(attrib, 1);
        byteCounter += layout.at(i)*sizeof(float);
    }
}

void VertexLoader::loadPoint(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices) {
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    SimpleVertex s(glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0), 0.0);
        
    glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleVertex), &s, GL_STATIC_DRAW);
    
    GLuint i = 0;
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint), &i, GL_STATIC_DRAW);
    
    numIndices = 1;
    setupVAOAttribs(VERTEX_SIMPLEVERTEX);
    glBindVertexArray(0); 
}

void VertexLoader::loadMapChunk(const std::string& src, int chunkX, int chunkY, int subdivisions, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices) {
    // DRAW A FRICKIN DIAGRAM IF U WANNA UNDERSTAND THIS
    
    int imageWidth = 0;
    int imageHeight = 0;
    int channels = 0;
    unsigned char* imageData;

    imageData = stbi_load(src.c_str(), &imageWidth, &imageHeight, &channels, 0);
    if (channels != 1) {
        return;
    }
    
    std::vector<Vertex> vertices;
    
    int chunkWidth = std::round(imageWidth / (float) subdivisions);
    int chunkHeight = std::round(imageHeight / (float) subdivisions);
    
    int pixelX = chunkWidth * chunkX; // chunkX starts at 0
    
    int pixelY = chunkHeight * chunkY;
    
    int startingIndex = pixelX + pixelY * chunkWidth;

    vertices.resize(chunkHeight*chunkWidth);  
    int c = 0;
    for (int i = 0; i < chunkHeight; i++) { 
        for (int j = 0; j < chunkWidth; j++) {
            int index = (startingIndex + j + i * chunkWidth); 
            int height = (int)imageData[index];
            float h = (float) height / 100.0f;
            Vertex v;
            float x= float(pixelX)+j;
            float z = float(pixelY) +i;
            v.Pos = glm::vec3(x, h,z);
            v.Normal = glm::vec3(0.0,1.0,0.0); 
            v.TexCoords = glm::vec2((float)std::remainder(j,2),(float)std::remainder(i,2));
            vertices[c] = v; // FIX DUMBASS
            c++; 
            printf("%i\n",height);
        }
    }
     
    std::vector<GLuint> indices;
    
    for (int i = 0; i < chunkHeight-1; i++) {
        for (int j = 0; j < chunkWidth-1; j++) {
        indices.push_back(0.0f+j+chunkWidth*i);
        indices.push_back(0.0f+j+chunkWidth*i+1);
        indices.push_back(0.0f+j+chunkWidth*i+chunkWidth);
        }
    }
    for (int i = 0; i < chunkHeight-1; i++) {
        for (int j = 0; j < chunkWidth-1; j++) {
        indices.push_back(0.0f+j+chunkWidth*i+1);
        indices.push_back(0.0f+j+chunkWidth*i+chunkWidth);
        indices.push_back(0.0f+j+chunkWidth*i+chunkWidth+1);
        }
    }
        /**
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            Vertex vertex;
            vertex.Pos = glm::vec3( (float)(2.0f*(-0.5f+i/49.0f)),2.0f*(float)(-0.5f+j/49.0f),
                                         (0.0f));
            vertex.Normal = glm::vec3(0,0,1);
     
            vertex.TexCoords = glm::vec2( (float)(0.0f+i/49.0f), (float)(0.0f+j/49.0f));
            
            vertices.push_back(vertex);
        }
    }
        
    std::vector<GLuint> indices;
    for (int i = 0; i < 49; i++) {
        for (int j = 0; j < 49; j++) {
        indices.push_back(0.0f+j+50*i);
        indices.push_back(0.0f+j+50*i+1);
        indices.push_back(0.0f+j+50*i+50);
        }
    }**/
    stbi_image_free(imageData);
    glBindVertexArray(vao);
     
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
        
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), indices.data(), GL_STATIC_DRAW);
    
    numIndices = indices.size();
    setupVAOAttribs(VERTEX_VERTEX);
    glBindVertexArray(0);
}
 
