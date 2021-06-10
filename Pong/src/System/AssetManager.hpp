//
//  AssetManager.hpp
//  Pong
//
//  Created by Joseph Gu on 5/18/21.
//

#ifndef AssetManager_hpp
#define AssetManager_hpp

#define TEX_ENERGY "Resources/Particles/aura.png"
#define TEX_PEACH "Resources/Particles/rosa copy.png"
#define TEX_FIERY "Resources/Particles/download.png"

#define TEX_GRADIENT "Resources/Utility/radial.png"

#define MOD_JUGGERNAUT "Resources/Models/juggernaut/juggernaut/materials/juggernaut_econ.fbx"
#define MOD_PHOENIX "Resources/Models/phoenix/phoenix_bird/materials/phoenix_bird_econ.fbx"

#define MOD_JUGGERNAUT_HBOX 0.5,-0.5,0.75,0.25,0.5,-0.5

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include "Mesh.hpp"
#include <vector>
#include "Model.hpp"
#include "stb_image.h"
#include <stdio.h>
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>

//loads textures and assets

static std::vector<Texture> loadedTextures;
static std::vector<Model*> loadedModels;

struct Character {
    GLuint id;
    glm::ivec2   size;       // Size of glyph
    glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
    GLuint advance; // advancing to next glph?
};

static int loadGlyphs(const char* filePath, std::map<char, Character>& Characters) {
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
        std::cout << glGetError();
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

static GLuint loadTexture(const char* filePath) {
   for (int j = 0; j < loadedTextures.size(); j++) {
       if (std::strcmp(loadedTextures[j].path.data(), filePath) == 0) {
           std::cout << loadedTextures[j].path.data() << "\n";
           return loadedTextures[j].id;
       }
   }
   GLuint texture;
   int imageWidth = 0;
   int imageHeight = 0;
   int channels = 0;
   unsigned char* imageData;
   
   glGenTextures(1, &texture);
   glBindTexture(GL_TEXTURE_2D, texture);
   
   imageData = stbi_load(filePath, &imageWidth, &imageHeight, &channels, 0);
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
   
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
   glBindTexture(GL_TEXTURE_2D, 0);
   Texture newTex;
   newTex.id = texture;
   newTex.path = filePath;
   loadedTextures.push_back(newTex);
   return texture;
}

static Model* loadModels(const char* filePath) {
    for (int j = 0; j < loadedModels.size(); j++) {
        if (std::strcmp(loadedModels[j]->getDirectory().data(), filePath) == 0) {
            return loadedModels[j];
        }
    }
    Model* model = new Model(filePath);
   /** Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath,  aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString();
        return NULL;
    }
    std::string filePath_ = (std::string)(filePath);
    filePath_ = filePath_.substr(0, filePath_.find_last_of('/'));
    model->setDirectory(filePath_.c_str());
    model->setMeshes(processNode(scene->mRootNode, scene));**/
    
    Hitbox hitbox{MOD_JUGGERNAUT_HBOX};
    model->setHitbox(hitbox);
    loadedModels.push_back(model);
    return model;
}

/**static std::vector<Mesh> processNode(aiNode* node, const aiScene* scene);

Mesh processMesh(aiMesh* mesh, const aiScene* scene);

std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type_, std::string typeName);**/
#endif /* AssetManager_hpp */

