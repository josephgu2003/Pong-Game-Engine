//
//  AssetManager.cpp
//  Pong
//
//  Created by Joseph Gu on 5/18/21.
//

#include <fstream>
#include "Dialogue.hpp"
#include "AssetManager.hpp"

extern GLuint uboViewProj;

extern GLuint uboLights;

extern GLuint uboStopWatch;

void AssetManager::loadTexture(const char* filePath, Texture* texture, bool srgb) {
 for (int j = 0; j < loadedTextures.size(); j++) {
       if (std::strcmp(loadedTextures[j].path.data(), filePath) == 0) {
           std::cout << "Already loaded:" << loadedTextures[j].path.data() << "\n";
           texture->id = loadedTextures[j].id;
           texture->path = loadedTextures[j].path;
        texture->textureTarget = GL_TEXTURE_2D;
         texture->dimensions = loadedTextures[j].dimensions;
           return;
       }
   }

   int imageWidth = 0;
   int imageHeight = 0;
   int channels = 0;
   unsigned char* imageData;
   
   glGenTextures(1, &texture->id);
   glBindTexture(GL_TEXTURE_2D, texture->id);
   
   imageData = stbi_load(filePath, &imageWidth, &imageHeight, &channels, 0);
   if (imageData) {
       GLenum format = GL_RGBA;
              if (channels == 1)
                  format = GL_RED;
              else if (channels == 3)
                  format = GL_RGB;
              else if (channels == 4)
                  format = GL_RGBA;
       if (srgb)
       glTexImage2D(GL_TEXTURE_2D, 0,GL_SRGB_ALPHA, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, imageData);
       else {
           glTexImage2D(GL_TEXTURE_2D, 0,format, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, imageData);
       }

       glGenerateMipmap(GL_TEXTURE_2D);
   } else {
       std::cout << "Failed to load texture data \n" << stbi_failure_reason() << "\n";
   }
    
   stbi_image_free(imageData);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
   glBindTexture(GL_TEXTURE_2D, 0);

   Texture newTex;
   newTex.id = texture->id;
   newTex.path = filePath;
    newTex.textureTarget = GL_TEXTURE_2D;
    newTex.dimensions = glm::vec3(imageWidth, imageHeight, 0);
   loadedTextures.push_back(newTex);
    
    texture->textureTarget = GL_TEXTURE_2D;
    texture->dimensions = newTex.dimensions;
}

void AssetManager::load3DTexture(const char* filePath, Texture* texture) {
    
    stbi_set_flip_vertically_on_load(0);
 
    for (int j = 0; j < loadedTextures.size(); j++) {
        if (std::strcmp(loadedTextures[j].path.data(), filePath) == 0) {
            std::cout << loadedTextures[j].path.data() << "\n";
            texture->id = loadedTextures[j].id;
            texture->path = loadedTextures[j].path;
         texture->textureTarget = GL_TEXTURE_2D_ARRAY;
          texture->dimensions = loadedTextures[j].dimensions;
            return;
        }
    }

    std::vector<unsigned char*> imageDatas;
    
    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture->id);
    
    int counter = 0;
    
    int imageWidth = 0;
    int imageHeight = 0;
    int channels = 0;
    
 
    while(true) {
        unsigned char* imageData;
        std::string framePath("blah");
        if(counter > 0) {
            std::string number= std::to_string(counter);
            if (counter < 10) {
                number = "";
                number += "00";
                number += std::to_string(counter);
            } else if (counter < 100) {
                number = "";
                number += "0";
                number += std::to_string(counter);
            }
            std::string filePath_ = std::string(filePath);
            framePath = filePath_.replace(filePath_.begin()+filePath_.find('.')-3,filePath_.begin()+filePath_.find('.'),number);;
           
        } else {
            framePath = std::string(filePath);
        }
    counter++;
        

    imageData = stbi_load(framePath.c_str(), &imageWidth, &imageHeight, &channels, 0);
        
    if (imageData) {
        imageDatas.push_back(imageData);
    } else {
        std::cout << "Failed to load texture data \n" << stbi_failure_reason() << "\n";
        stbi_image_free(imageData);
        break;
    }
    }
    
    GLenum format = GL_RGBA;
           if (channels == 1)
               format = GL_RED;
           else if (channels == 3)
               format = GL_RGB;
           else if (channels == 4)
               format = GL_RGBA;
    
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0,GL_RGBA8, imageWidth, imageHeight, counter-1, 0, format, GL_UNSIGNED_BYTE, NULL);
    
   for(int i = 0; i < imageDatas.size(); i++) {
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, imageWidth, imageHeight, 1, format, GL_UNSIGNED_BYTE, imageDatas.at(i));
    }
   for(int i = 0; i < imageDatas.size(); i++) {
       stbi_image_free(imageDatas.at(i));
     }
  //  imageDatas.clear();
    // 460, 271.4, 233.4, 234
    
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
    Texture newTex;
    newTex.id = texture->id;
    newTex.path = filePath;
    newTex.dimensions = glm::vec3(imageWidth, imageHeight, imageDatas.size());
    newTex.textureTarget = GL_TEXTURE_2D_ARRAY;
    loadedTextures.push_back(newTex);
    stbi_set_flip_vertically_on_load(0);
    glBindTexture(GL_TEXTURE_2D_ARRAY,0);

    texture->path = filePath;
 texture->textureTarget = GL_TEXTURE_2D_ARRAY;
  texture->dimensions = newTex.dimensions;
}

int AssetManager::loadGlyphs(const char* filePath, std::map<char, Character>& Characters) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }
    FT_Face face;
    if (FT_New_Face(ft, filePath, 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    FT_Set_Pixel_Sizes(face, 0, 192);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    for (unsigned char c = 0; c < 126; c++) {
        if(FT_Load_Char(face, (char)c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
            continue;
        }
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Character character = {
            texture, glm::ivec2(face->glyph->bitmap.width,face->glyph->bitmap.rows),glm::ivec2(face->glyph->bitmap_left,face->glyph->bitmap_top), (GLuint)face->glyph->advance.x
        };
        Characters.insert(std::pair<char,Character>(c, character));
    }
         FT_Done_Face(face);
         FT_Done_FreeType(ft);
    return 0;
}


void AssetManager::loadNullTexture(int x, int y, GLuint* texture, GLenum format) {
    
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    
        glTexImage2D(GL_TEXTURE_2D, 0,format, x, y, 0, format, GL_UNSIGNED_BYTE, NULL);
        glGenerateMipmap(GL_TEXTURE_2D);
     
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void AssetManager::bindShaderUniblock(Shader* shader, Uniblock block) {
    if (block == ViewProj) {
    glBindBuffer(GL_UNIFORM_BUFFER, uboViewProj);
    glUniformBlockBinding(shader->ID, glGetUniformBlockIndex(shader->ID, "ViewProj"), 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboViewProj);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
 
    if (block == Lights) {
    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
    glUniformBlockBinding(shader->ID, glGetUniformBlockIndex(shader->ID, "Lights"), 1);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboLights);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
    
    if (block == StopWatch) {

    glBindBuffer(GL_UNIFORM_BUFFER, uboStopWatch);
    glUniformBlockBinding(shader->ID, glGetUniformBlockIndex(shader->ID, "StopWatch"), 2);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboStopWatch);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}


void AssetManager::buildTree(DialogueTree*& tree, int i, int branchID) {
    if (branchID == -1)  {
        tree = nullptr;
        return;
    }
    for (int j = 0; j < dialogues.at(i)["Dialogue"].size(); j++) {
        if (dialogues.at(i)["Dialogue"].at(j)["LineID"] == branchID) { tree->lines.insert(tree->lines.end(),       dialogues.at(i)["Dialogue"].at(j)["Lines"].begin(), dialogues.at(i)["Dialogue"].at(j)["Lines"].end());
        tree->left = new DialogueTree(); //interesting, these 2 lines are necessary or else original tree back in loadDialogue is different pointer than the tree in the first buildtree call
        tree->right = new DialogueTree();
        buildTree((tree->left), i, dialogues.at(i)["Dialogue"].at(j)["LeftID"]);
        buildTree((tree->right), i, dialogues.at(i)["Dialogue"].at(j)["RightID"]);
        return;
        } 
    }
}

void AssetManager::loadDialogue(Dialogue* dialogue, int id) {
    if (dialogues.size() == 0) { //check if loaded
        std::ifstream i("Data/Scenes/File.json");
        i >> dialogues;
    }
    
    DialogueTree* tree = new DialogueTree();
    
    for (int i = 0; i < dialogues.size(); i++) {
        if (dialogues.at(i)["DialogueID"] == i) { // find right dialogue
            buildTree(tree, i, 0);
        }
    }
    
    dialogue->setDialogueTree(tree);
}
/**
static std::vector<Mesh> processNode(aiNode* node, const aiScene* scene) {
    //load each mesh from node
    std::vector<Mesh> meshes;
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        std::cout << meshes.size() << "\n";
        meshes.push_back(processMesh(mesh, scene));
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
    return meshes;
}

Mesh processMesh(aiMesh* mesh, const aiScene* scene) {5
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) { //iterate over mesh vertices
        Vertex vertex;
        
        glm::vec3 pos_;
        pos_.x = mesh->mVertices[i].x;
        pos_.y = mesh->mVertices[i].y;
        pos_.z = mesh->mVertices[i].z;
        vertex.Pos = pos_;
        
        if (mesh->mTextureCoords[0]) {
            glm::vec2 texCoords_;
            texCoords_.x = mesh->mTextureCoords[0][i].x;
            texCoords_.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = texCoords_;
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        
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
    Mesh returnedMesh(vertices, indices, textures);
    return returnedMesh;
}

std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type_, std::string typeName) {
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

GLuint getTextureFromFile(std::string texturePath_) {
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
**/
