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

BoneList VertexLoader::activeBoneList;

std::map<std::string, BoneList> VertexLoader::loadedBoneLists;
std::map<std::string, bool> VertexLoader::bonesReferencedByMeshes;
int VertexLoader::indexOffset = 0;
std::map<std::string, VertexData> VertexLoader::loadedVertexData;

void VertexLoader::loadTextDataAbstraction(const std::string& text, float fontsize, float linespace, float maxlinelength, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, Material& map, glm::vec2 position, bool useMaxLineLength) {
    glBindVertexArray(vao);
    
    std::map<char, Character> characters;
    
    AssetManager::loadGlyphs("Resources/GlyphsAndUI/times.ttf", characters, map);
    
    std::string::const_iterator c;
    float x = position.x;
    float y = position.y;
    float scale = fontsize;
    
    std::vector<SimpleVertex> newVertices;
    std::vector<GLuint> newIndices;
    
    int indexOffset = 0;
    
    float currentLineLength = 0.0f;
    
    for (c = text.begin(); c != text.end(); c++)
    {
        if (*c == '\n') {
            x = position.x;
            y -= linespace*scale;
            currentLineLength = 0.0f;
            continue;
        }
        Character ch = characters[*c];
        
        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;
        
        float w = ch.size.x * scale;
        currentLineLength += w;
        
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
        if (useMaxLineLength) {
        if (currentLineLength > maxlinelength && *c == ' ') {
            x = position.x;
            y -= linespace*scale;
            currentLineLength = 0.0f;
        }
        }
    }
    
    fillVertexData<SimpleVertex>(vao, vbo, ebo, numIndices, GL_STATIC_DRAW, GL_STATIC_DRAW, newVertices, newIndices);
}

void VertexLoader::loadTextData(const std::string& text, float fontsize, float linespace, float maxlinelength, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, Material& map, glm::vec2 position) {
    loadTextDataAbstraction(text, fontsize, linespace, maxlinelength, vao, vbo, ebo, numIndices, map, position, true);
}
void VertexLoader::loadTextData(const std::string& text, float fontsize, float linespace, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, Material& map, glm::vec2 position) {
    loadTextDataAbstraction(text, fontsize, linespace, 0, vao, vbo, ebo, numIndices, map, position, false);
}

void VertexLoader::setupVAOAttribs(VertexType vt) {
    auto setVertexAttribsFloat = [] (int vertexSize, std::vector<int>& numFloats, std::vector<int>& offsets) {
        if (numFloats.size() == offsets.size()) {
            for (int i = 0; i < numFloats.size(); i++) {
                glVertexAttribPointer(i, numFloats.at(i), GL_FLOAT, GL_FALSE,vertexSize, (void*)(offsets.at(i)));
                glEnableVertexAttribArray(i);
            }
        }
    };
    
    switch (vt) {
        case VERTEX_SIMPLEVERTEX: {
            std::vector<int> numFloats = {3,2,1};
            std::vector<int> offsets = {0, (sizeof(glm::vec3)),(sizeof(glm::vec2) + sizeof(glm::vec3))};
            setVertexAttribsFloat(sizeof(SimpleVertex), numFloats, offsets);
            break;
        }
            
        case VERTEX_VERTEX: {
            std::vector<int> numFloats = {3,3,2};
            std::vector<int> offsets = {0, (sizeof(glm::vec3)),(2*sizeof(glm::vec3))};
            setVertexAttribsFloat(sizeof(Vertex), numFloats, offsets);
            break;
        }
            
        case VERTEX_TBNVERTEX: {
            std::vector<int> numFloats = {3,3,2,3,3};
            std::vector<int> offsets = {0, (sizeof(glm::vec3)),(2*sizeof(glm::vec3)), (2*sizeof(glm::vec3)+sizeof(glm::vec2)),(3*sizeof(glm::vec3)+sizeof(glm::vec2))};
            setVertexAttribsFloat(sizeof(TBNVertex), numFloats, offsets);
            break;
        }
            
        case VERTEX_TBNBWVERTEX: {
            std::vector<int> numFloats = {3,3,2,3,3};
            std::vector<int> offsets = {0, (sizeof(glm::vec3)),(2*sizeof(glm::vec3)), (2*sizeof(glm::vec3)+sizeof(glm::vec2)),(3*sizeof(glm::vec3)+sizeof(glm::vec2))};
            setVertexAttribsFloat(sizeof(TBNBWVertex), numFloats, offsets);
            
            glVertexAttribIPointer(5, MAX_BONE_WEIGHTS, GL_INT, sizeof(TBNBWVertex), (void*)(4*sizeof(glm::vec3)+sizeof(glm::vec2)));
            glEnableVertexAttribArray(5);
            
            glVertexAttribPointer(6, MAX_BONE_WEIGHTS, GL_FLOAT, GL_FALSE, sizeof(TBNBWVertex), (void*)(MAX_BONE_WEIGHTS*sizeof(int)+4*sizeof(glm::vec3)+sizeof(glm::vec2)));
            glEnableVertexAttribArray(6);
            
            break;
        }
            
    }
}

void VertexLoader::load2DQuadData(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices, glm::vec2 dimensions, glm::vec2 position) {
    
    std::vector<SimpleVertex> newVertices = {SimpleVertex(glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0), 0.0), SimpleVertex(glm::vec3(0.0, 1.0, 0.0), glm::vec2(0.0, 1.0), 0.0), SimpleVertex(glm::vec3(1.0, 0.0, 0.0), glm::vec2(1.0, 0.0), 0.0), SimpleVertex(glm::vec3(1.0, 1.0, 0.0), glm::vec2(1.0, 1.0), 0.0)};
    
    for (int i = 0; i < newVertices.size(); i++) {
        SimpleVertex& sv = newVertices.at(i);
        sv.Pos.x *= dimensions.x;
        sv.Pos.y *= dimensions.y;
        //    sv.Pos += glm::vec3(position.x, position.y, 0.0f);
    }
    
    std::vector<GLuint> indices = {
        0, 1, 3, 0, 2, 3
    };
    
    fillVertexData<SimpleVertex>(vao, vbo, ebo, numIndices, GL_STATIC_DRAW, GL_STATIC_DRAW, newVertices, indices);
}
 
bool VertexLoader::loadAlreadyLoadedData(const std::string& name, unsigned int& vao, unsigned int& vbo, unsigned int& ebo, unsigned int& numIndices) {
    auto entry = loadedVertexData.find(name);
    if (entry != loadedVertexData.end()) {
        auto data = entry->second;
        vao = data.vao;
        vbo = data.vbo;
        ebo = data.ebo;
        numIndices = data.numIndices;
        return true;
    }
    return false;
}

void VertexLoader::loadModelSimple(std::string filePath, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices) {
    bool dummy = false;
    loadModelAbstraction<SimpleVertex>(filePath, vao, vbo, ebo, numIndices, dummy, false);
}

void VertexLoader::loadModel(std::string filePath_, unsigned int& vao, unsigned int& vbo, unsigned int& ebo, unsigned int& numIndices, bool& deleteDataOnDestruct) {
    loadModelAbstraction<TBNBWVertex>(filePath_, vao, vbo, ebo, numIndices, deleteDataOnDestruct, true);
}  

void VertexLoader::loadModelAnimations(AnimComponent* anim_, std::vector<BoneNode>& boneNodes, std::string filePath_) {
    
    Assimp::Importer importer;
    importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    
    const aiScene* scene = importer.ReadFile(filePath_,  aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace); 
     
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::string s = "ERROR::ASSIMP::" + std::string(importer.GetErrorString()) + "\n";
        printf("%s", s.c_str());
        return;
    }
    
    if (loadedBoneLists.find(filePath_) == loadedBoneLists.end()) {
        readAssimpTree(scene->mRootNode, boneNodes);
        
        std::vector<TBNBWVertex> vertices;
        std::vector<GLuint> indices;
        processNode(scene->mRootNode, scene, vertices, indices);
         
        boneNodes = activeBoneList;
         
        if (activeBoneList.size() != 0) loadedBoneLists.insert(std::pair<std::string, BoneList>(filePath_, activeBoneList));
    } else {
        boneNodes = loadedBoneLists.find(filePath_)->second;
    }
 
    for (int i = 0; i < scene->mNumAnimations; i ++) { 
        anim_->addAnimation(scene->mAnimations[i], scene);
    }
    
    reset();
} 

void VertexLoader::processMesh(aiMesh* mesh, const aiScene* scene, std::vector<SimpleVertex>& vertices, std::vector<GLuint>& indices) {
    
    std::vector<SimpleVertex> newVertices;
    newVertices.reserve(mesh->mNumVertices);
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) { //iterate over mesh vertices
        glm::vec3 pos_;
        pos_.x = mesh->mVertices[i].x;
        pos_.y = mesh->mVertices[i].y;
        pos_.z = mesh->mVertices[i].z;
        
        glm::vec2 texCoords_;
        if (mesh->mTextureCoords[0]) {
            texCoords_.x = mesh->mTextureCoords[0][i].x;
            texCoords_.y = mesh->mTextureCoords[0][i].y;
        } else {
            texCoords_ = glm::vec2(0.0f, 0.0f);
        }
        
        SimpleVertex v(pos_, texCoords_, 0.0f);
        newVertices.push_back(v);
    }
    
    indices.reserve(indices.size() + mesh->mNumFaces);
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) { // iterate over mesh faces
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            GLuint ind = face.mIndices[j] + indexOffset;
            indices.push_back(ind);
        }
    }
    vertices.insert(vertices.end(), newVertices.begin(), newVertices.end());
    indexOffset += mesh->mNumVertices;
}

void VertexLoader::processMesh(aiMesh* mesh, const aiScene* scene, std::vector<TBNBWVertex>& vertices, std::vector<GLuint>& indices) {
    auto loadVec3 = [] (glm::vec3& vec3, aiVector3D* vec3src, int i) {
        vec3.x = vec3src[i].x;
        vec3.y = vec3src[i].y;
        vec3.z = vec3src[i].z;
    };
    std::vector<TBNBWVertex> newVertices; 
    newVertices.reserve(mesh->mNumVertices);
     
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) { //iterate over mesh vertices
        glm::vec3 pos_;
        loadVec3(pos_, mesh->mVertices, i);
        
        glm::vec3 norm_;
        loadVec3(norm_, mesh->mNormals, i);
        
        glm::vec3 Tan_;
        loadVec3(Tan_, mesh->mTangents, i);
        
        glm::vec3 BiTan_;
        loadVec3(BiTan_, mesh->mBitangents, i);
        
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
 
        newVertices.emplace_back(pos_, norm_, texCoords_, Tan_, BiTan_, boneIDs, boneWeights);
    }
    
    indices.reserve(indices.size() + mesh->mNumFaces);
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) { // iterate over mesh faces
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            GLuint ind = face.mIndices[j] + indexOffset;
            indices.push_back(ind);
        }
    }
    
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        Material m;
        loadMaterialTextures(m, material);
    }
    if (activeBoneList.size() != 0) {
        BoneWeightVertices(newVertices, mesh, scene);
    }  
    vertices.insert(vertices.end(), newVertices.begin(), newVertices.end());
    indexOffset += mesh->mNumVertices;
}

void VertexLoader::loadMaterialTextures(Material& mat, aiMaterial* material) {
        aiString string;
    material->GetTexture(aiTextureType_DIFFUSE, 0, &string);
    std::string string_ = (std::string)string.C_Str();
    std::string fileName = string_.substr(string_.find_last_of("\\")+ 1);
    
    AssetManager::loadTexture(fileName.c_str(), &mat.diffuse, true);

    material->GetTexture(aiTextureType_NORMALS, 0, &string);
    string_ = (std::string)string.C_Str();
    fileName = string_.substr(string_.find_last_of("\\")+ 1);
     
    AssetManager::loadTexture(fileName.c_str(), &mat.normMap, false);
}

void VertexLoader::BoneWeightVertices(std::vector<TBNBWVertex>& vertices, aiMesh* mesh,
                                      const aiScene* scene) {
    int x = mesh->mNumBones;
     
    auto findBone = [&] (const std::string name, int& index) -> BoneNode* {
        for (int i = 0; i < activeBoneList.size(); i++) {
            BoneNode& node = activeBoneList.at(i);
            if (node.name == name) {
                index = i;
                return &node;
            }
        }
        return nullptr;
    };
    
    // iterate over number of bones in this mesh
    for (int boneIndex = 0; boneIndex < x; ++boneIndex) {
        int id = -1; // represents the location in the BoneMap
        std::string boneName(mesh->mBones[boneIndex]->mName.C_Str());
        
        BoneNode* boneNode = findBone(boneName, id);
        
        if (boneNode == nullptr)
        { 
            throw "Mesh referenced bone not found in Asssimp Node hierarchy.";
        } else {
            glm::mat4 offsetMatrix;
            ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix, offsetMatrix);
            boneNode->offset = offsetMatrix;
            
            if (bonesReferencedByMeshes.find(boneName)->second && boneNode->offset != offsetMatrix) {
                throw "Error : %s offset differs across meshes \n";
            }
            bonesReferencedByMeshes.find(boneName)->second = true;
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
    
    // normalize vertex weights
    for (int i = 0; i < vertices.size(); i++) {
        float sum = 0.0f;
        
        // get the sum of the weights
        for (int j =0 ; j < MAX_BONE_WEIGHTS; j++) {
            if (vertices[i].boneIDs[j] != -1)
                sum += vertices[i].boneWeights[j];
        }
        float factor = 1.0f / sum;
        // normalize
        for (int j =0 ; j < MAX_BONE_WEIGHTS; j++) {
            if (vertices[i].boneIDs[j] != -1)
                vertices[i].boneWeights[j] *= factor;
        }
    }
}

void VertexLoader::ConvertMatrixToGLMFormat(const aiMatrix4x4& from, glm::mat4& to)
{
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            to[x][y] = from[y][x];
        }
    }
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
    activeBoneList.clear();
    indexOffset = 0;
    bonesReferencedByMeshes.clear();
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

void VertexLoader::incTanBitanForTriangle(TBNVertex& v1, TBNVertex& v2, TBNVertex& v3) {
    glm::vec3 p1 = v1.Pos;
    glm::vec3 p2 = v2.Pos;
    glm::vec3 p3 = v3.Pos;
    
    glm::vec2 uv1 = v1.TexCoords;
    glm::vec2 uv2 = v2.TexCoords;
    glm::vec2 uv3 = v3.TexCoords;
    
    glm::vec3 edge1 = p2 - p1;
    glm::vec3 edge2 = p3 - p1;
    
    glm::vec2 dUV1 = uv2 - uv1;
    glm::vec2 dUV2 = uv3 - uv1;
    
    float f = 1.0f / (dUV1.x*dUV2.y - dUV2.x * dUV1.y);
    
    glm::vec3 tan, bitan;
    tan = f * (dUV2.y * edge1 - dUV1.y * edge2);
    bitan = f * (- dUV2.x * edge1 + dUV1.x * edge2);
    
    
    auto incTanBitan = [] (TBNVertex& v, glm::vec3& t, glm::vec3& b) {
        v.Tan += t;
        v.BiTan += b;
    };
    
    incTanBitan(v1, tan, bitan);
    incTanBitan(v2, tan, bitan);
    incTanBitan(v3, tan, bitan);
}

void VertexLoader::loadPoint(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices) {
    std::vector<SimpleVertex> vertices;
    vertices.emplace_back(glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0), 0.0);
    std::vector<GLuint> indices;
    indices.push_back(0);
    
    fillVertexData<SimpleVertex>(vao, vbo, ebo, numIndices, GL_STATIC_DRAW, GL_STATIC_DRAW, vertices, indices);
}


void VertexLoader::loadMapChunk(float heightMesh[CHUNK_DIM_PXLS][CHUNK_DIM_PXLS],const unsigned short* heightMap, int imageWidth, int imageHeight, int chunkX, int chunkY, glm::vec2 originPos, glm::vec3 scaling, unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices) {
    // DRAW A FRICKIN DIAGRAM IF U WANNA UNDERSTAND THIS
    
    int maxRows = CHUNK_DIM_PXLS;
    int maxColumns = CHUNK_DIM_PXLS;
    int imageArea = imageWidth*imageHeight;
    
    if (std::floor(float(imageWidth-1)/(CHUNK_DIM_PXLS-1)) == chunkX) {
        maxColumns--;
    }
    if (std::floor(float(imageHeight-1)/(CHUNK_DIM_PXLS-1)) == chunkY) {
        maxRows--;
    }
    
    int startingIndex = chunkX*(CHUNK_DIM_PXLS-1) + chunkY * (CHUNK_DIM_PXLS -1) * imageWidth;
    int maxIndex = startingIndex + (maxColumns-1)+(maxRows-1)*imageWidth;
    
    std::vector<TBNVertex> vertices;
    vertices.resize((CHUNK_DIM_PXLS+2)*(2+CHUNK_DIM_PXLS));
    
    float xCoord = originPos.x;
    float yCoord = originPos.y;
    
    float scaleY = scaling.y;
    
    int startingIndexExpanded = startingIndex - imageWidth - 1;
    if (startingIndexExpanded > 0) {
        startingIndex = startingIndexExpanded;
    }
    int maxIndexExpanded = maxIndex+1+imageWidth;
    if (maxIndexExpanded < imageArea) {
        maxIndex = maxIndexExpanded;
    }// we want an extra layer of vertices around the chunk for generating normals
    
    auto fetchHeightAt = [=] (int i, int j, const unsigned short* data) {
        int index = (startingIndex + j + i * imageWidth);
        if (index < 0 || index > maxIndex) {
            return 0.0f;
        }
        float height = float(data[index]);
        float h = height * scaleY; // possible undefined behaviour
        return h;
    };
    
    
    int c = 0;
    
    for (int i = 0; i < (CHUNK_DIM_PXLS+2); i++) {
        for (int j = 0; j < (CHUNK_DIM_PXLS+2); j++) {
            TBNVertex v;
            float x = xCoord+j*scaling.x;
            float h = fetchHeightAt(i, j, heightMap);
            float z = yCoord +i*scaling.z;
            
            v.Pos = glm::vec3(x,h,z);
            v.Normal = glm::vec3(0,0,0);
            // v.TexCoords = glm::vec2(0.5f*std::fmod(j,2),0.5f*std::fmod(i,2));
            v.TexCoords = glm::vec2(float(i)/(CHUNK_DIM_PXLS+2), float(j)/(CHUNK_DIM_PXLS+2));
            v.Tan = glm::vec3(0,0,0);
            v.BiTan = glm::vec3(0,0,0);
            
            vertices[c] = v; // FIX DUMBASS
            c++;
            
            if (i < (CHUNK_DIM_PXLS+1) && j < (CHUNK_DIM_PXLS+1) && i > 0 && j >0) {
                heightMesh[j-1][i-1] = h;
            }
        }
    }
    for (int i = 1; i < (CHUNK_DIM_PXLS+1); i++) {
        for (int j = 1; j < (CHUNK_DIM_PXLS+1); j++) {
            float h = vertices[j+i*(CHUNK_DIM_PXLS+2)].Pos.y;
            float hUp = vertices[j+(i-1)*(CHUNK_DIM_PXLS+2)].Pos.y;
            float hDown = vertices[j+(1+i)*(CHUNK_DIM_PXLS+2)].Pos.y;
            float hRight = vertices[1+j+i*(CHUNK_DIM_PXLS+2)].Pos.y;
            float hLeft = vertices[j-1+i*(CHUNK_DIM_PXLS+2)].Pos.y;
            vertices[j+i*(CHUNK_DIM_PXLS+2)].Normal = calcNormalWithHeights(h, hDown, hUp, hRight, hLeft);
        }
    }
    
    for (int i = 0; i < CHUNK_DIM_PXLS+1; i++) {
        for (int j = 0; j < CHUNK_DIM_PXLS+1; j++) {
            int dim = (CHUNK_DIM_PXLS+2);
            int botL = j+dim*i+dim;
            int botR = j+dim*i+dim+1;
            int topL = j+dim*i;
            int topR = j+dim*i+1;
            incTanBitanForTriangle(vertices.at(botL), vertices.at(topR), vertices.at(topL));
            incTanBitanForTriangle(vertices.at(botL),vertices.at(botR), vertices.at(topR));
        }
    }
    
    std::vector<TBNVertex> newVertices;
    newVertices.resize(CHUNK_DIM_PXLS*CHUNK_DIM_PXLS);
    int counter = 0;
    for (int i = 0; i < (CHUNK_DIM_PXLS+2); i++) {
        for (int j = 0; j < (CHUNK_DIM_PXLS+2); j++) {
            if (i < (CHUNK_DIM_PXLS+1) && j < (CHUNK_DIM_PXLS+1) && i > 0 && j >0) {
                newVertices[counter] = vertices[j+i*(CHUNK_DIM_PXLS+2)];
                counter++;
            }
        }
    }
    normalizeTanBitan(newVertices);
    
    std::vector<GLuint> indices;
    indices.reserve((CHUNK_DIM_PXLS-1)*(CHUNK_DIM_PXLS-1));
    
    for (int i = 0; i < CHUNK_DIM_PXLS-1; i++) {
        for (int j = 0; j < CHUNK_DIM_PXLS-1; j++) {
            int dim = (CHUNK_DIM_PXLS);
            int botL = j+dim*i+dim;
            int botR = j+dim*i+dim+1;
            int topL = j+dim*i;
            int topR = j+dim*i+1;
            indices.push_back(botL); // bottom left
            indices.push_back(topR); // top right
            indices.push_back(topL); // top left
            indices.push_back(botL); // bottom left
            indices.push_back(botR); // bottom right
            indices.push_back(topR); // top right
        }
    }
    
    fillVertexData<TBNVertex>(vao, vbo, ebo, numIndices, GL_STATIC_DRAW, GL_STATIC_DRAW, newVertices, indices);
}
//problems : graphics component is responsible for creating meshes too?
void VertexLoader::normalizeTanBitan(std::vector<TBNVertex>& vertices) {
    int c = 0;
    for (TBNVertex& vertex : vertices) {
        glm::vec3& norm = vertex.Normal;
        glm::vec3& tan = vertex.Tan; 
        glm::vec3& bitan = vertex.BiTan;
        
        glm::vec3 newtan = tan - (norm * glm::dot(norm, tan));
        newtan = glm::normalize(tan);
        
        glm::vec3 newbitan = glm::cross(norm, newtan);
        if (dot(bitan,newbitan) < 0) {
            // newbitan = newbitan * -1.0f;
            glm::cross(newtan,norm);
        }
        newbitan = glm::normalize(newbitan);
        
        c++;
    }
}
glm::vec3 VertexLoader::calcNormalWithHeights(float cH, float dH, float uH, float rH, float lH) {
    glm::vec3 normal = glm::vec3(lH - rH, 2.0, dH - uH);
    normal = glm::normalize(normal);
    return normal;
}

void VertexLoader::loadSimpleVertexGrid(int verticesX, int verticesY, float scale, std::vector<PosVertex>& mesh, unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int& numIndices) { // will load the grid upright and in the xy plane
    std::vector<SimpleVertex> vertices;
    vertices.resize(verticesX * verticesY);
    mesh.resize(verticesX * verticesY);
    
    int c = 0;
    for (int i = 0; i < verticesY; i++) {
        for (int j = 0; j < verticesX; j++) {
            SimpleVertex v;
            v.Pos = glm::vec3(scale*(-0.5f+j/float(verticesX-1)), scale*(-0.5f+i/float(verticesY - 1)), 0.0f);
            v.TexCoords = glm::vec2(j/float(verticesX-1), i/float(verticesY-1));
            v.arraytexID = 0;
            vertices[c] = v; // FIX DUMBASS
            mesh[c] = PosVertex(v.Pos);
            c++;
        }
    }
    
    std::vector<GLuint> indices;
    
    for (int i = 0; i < verticesY-1; i++) {
        for (int j = 0; j < verticesX-1; j++) {
            indices.push_back(0.0f+j+verticesX*i+verticesX);
            indices.push_back(0.0f+j+verticesX*i+1);
            indices.push_back(0.0f+j+verticesX*i);
            indices.push_back(0.0f+j+verticesX*i+1);
            indices.push_back(0.0f+j+verticesX*i+verticesX);
            indices.push_back(0.0f+j+verticesX*i+verticesX+1);
        }
    }
    
    fillVertexData<SimpleVertex>(VAO, VBO, EBO, numIndices, GL_DYNAMIC_DRAW, GL_DYNAMIC_DRAW, vertices, indices);
}


static const float skyVertices[] = {
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    
    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};

void VertexLoader::loadSimpleCube(unsigned int vao, unsigned int vbo, unsigned int ebo, unsigned int& numIndices) { // fix this
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*108, &skyVertices[0], GL_STATIC_DRAW);
    
    std::vector<GLuint> indices;
    for (unsigned int i = 0; i < 36; i++) {
        indices.push_back(i);
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), indices.data(), GL_STATIC_DRAW);
    
    numIndices = indices.size();
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
}

/**const aiNode* VertexLoader::findRootBone(const aiScene* scene) {
    std::vector<const aiNode*> nodesNextLvl; // precursors
    std::vector<const aiNode*> buffer;
    
    nodesNextLvl.push_back(scene->mRootNode);
    
    while (!nodesNextLvl.empty()){
        for (int i = 0; i < nodesNextLvl.size(); i++) {
            const aiNode* node = nodesNextLvl.at(i);
            
            if (map_.find(node->mName.C_Str()) != map_.end()) {
                return node;
            }
            
            if (node->mChildren == NULL) {
                continue;
                
            }
            
            for (int j = 0; j < node->mNumChildren; j ++) { //buffer next lvl
                const      aiNode* precursor =
                node->mChildren[j];
                buffer.push_back(precursor);
            }
        }
        nodesNextLvl = buffer;
        buffer.clear();
    }
    return nullptr;
}
**/
void VertexLoader::readAssimpTree(const aiNode* node, std::vector<BoneNode>& boneNodes) {
    std::queue<const aiNode*> nodes;
    std::queue<int> parentIndices;
    
    nodes.push(node);
    parentIndices.push(-1);
    
    while (!nodes.empty()) {
        const aiNode* aNode = nodes.front();
        nodes.pop();
        int parentIndex = parentIndices.front();
        parentIndices.pop();
        
        std::string bonename = aNode->mName.C_Str();
        
        activeBoneList.emplace_back(bonename, aNode->mTransformation, parentIndex);
        bonesReferencedByMeshes.insert(std::pair<std::string, bool>(bonename, false));
        int thisIndex = boneNodes.size()-1;
        
        for (int i = 0; i < aNode->mNumChildren; i++) {
            nodes.push(aNode->mChildren[i]);
            parentIndices.push(thisIndex);
        }
    }

}
 
 
