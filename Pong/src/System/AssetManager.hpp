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
#define TEX_EMPTY "Resources/Particles/BLANK_ICON3.png"
#define TEX_INKPAPER "Resources/Textures/inkpaper.jpg"
#define TEX_MIST "Resources/Particles/imageedit_7_4243532340.png"

#define TEX_GRADIENT "Resources/Utility/radial.png"

#define MOD_JUGGERNAUT "Resources/Models/juggernaut/juggernaut/materials/juggernaut_econ.fbx"
#define MOD_PHOENIX "Resources/Models/phoenix/phoenix_bird/materials/phoenix_bird_econ.fbx"
#define MOD_HOODY "Resources/Models/journey5.obj"

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
#include "Shader.hpp"

//loads textures and assets

inline std::vector<Texture> loadedTextures;
inline std::vector<Model*> loadedModels;
inline std::vector<Shader> loadedShaders;

struct Character {
    GLuint id;
    glm::ivec2   size;       // Size of glyph
    glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
    GLuint advance; // advancing to next glph?
};

static void loadShader() {
    
}

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

static void load3DTexture(const char* filePath, GLuint& id) {
    
    stbi_set_flip_vertically_on_load(0);
 
    for (int j = 0; j < loadedTextures.size(); j++) {
        if (std::strcmp(loadedTextures[j].path.data(), filePath) == 0) {
            std::cout << loadedTextures[j].path.data() << "\n";
            id = loadedTextures[j].id;
        }
    }

    std::vector<unsigned char*> imageDatas;
    
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D_ARRAY, id);
    
    int counter = 0;
    
    int imageWidth = 0;
    int imageHeight = 0;
    int channels = 0;
    
    unsigned char* imageData;
    
    while(true) {
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
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    imageData = stbi_load(framePath.c_str(), &imageWidth, &imageHeight, &channels, 0);
        
    if (imageData) {
        imageDatas.push_back(imageData);
    } else {
        std::cout << "Failed to load texture data \n" << stbi_failure_reason() << "\n";
        stbi_image_free(imageData);
        break;
    }
  //  stbi_image_free(imageData);
    }

    GLenum format;
           if (channels == 1)
               format = GL_RED;
           else if (channels == 3)
               format = GL_RGB;
           else if (channels == 4)
               format = GL_RGBA;

      
    
   // glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, format, imageWidth, imageHeight, counter);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, format, imageWidth, imageHeight, counter-1, 0, format, GL_UNSIGNED_BYTE, NULL);
    // Upload pixel data.
    // The first 0 refers to the mipmap level (level 0, since there's only 1)
    // The following 2 zeroes refers to the x and y offsets in case you only want to specify a subrectangle.
    // The final 0 refers to the layer index offset (we start from index 0 and have 2 levels).
    // Altogether you can specify a 3D box subset of the overall texture, but only one mip level at a time.
    
   for(int i = 0; i < imageDatas.size(); i++) {
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, imageWidth, imageHeight, 1, format, GL_UNSIGNED_BYTE, imageDatas.at(i));
    }
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    // Always set reasonable texture parameters
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    Texture newTex;
    newTex.id = id;
    newTex.path = filePath;
    loadedTextures.push_back(newTex);
    stbi_set_flip_vertically_on_load(1);
 
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

