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
#define TEX_FISH "Resources/Models/Flag/fish.png"
#define TEX_SMOKES "Resources/Particles/Smokes/smoke000.jpg"
#define TEX_DANCE "Resources/Textures/Dance/dance000.png"
#define TEX_GRADIENT "Resources/Utility/radial.png"

#define MOD_JUGGERNAUT "Resources/Models/juggernaut/juggernaut/materials/juggernaut_econ.fbx"
#define MOD_PHOENIX "Resources/Models/phoenix/phoenix_bird/materials/phoenix_bird_econ.fbx"
#define MOD_HOODY "Resources/Models/journey7.obj"

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
inline std::vector<Shader> loadedShaders;


struct Character {
    GLuint id;
    glm::ivec2   size;       // Size of glyph
    glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
    GLuint advance; // advancing to next glph?
};

struct Frame {
    GLuint fbo;
    GLuint fvao;
    GLuint fvbo;
    GLuint frbo;
    GLuint ftexture;
    Shader shader;
};

struct DoubleFrame {
    GLuint fbo;
    GLuint fvao;
    GLuint fvbo;
    GLuint frbo;
    GLuint ftexture0;
    GLuint ftexture1;
    Shader shader;
};

static void loadShader() {
    
}
static void loadNullTexture(int x, int y, GLuint* texture, GLenum format) {
    int imageWidth = 0;
    int imageHeight = 0;
    int channels = 0;
    
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
    
    stbi_set_flip_vertically_on_load(1);
 
    for (int j = 0; j < loadedTextures.size(); j++) {
        if (std::strcmp(loadedTextures[j].path.data(), filePath) == 0) {
            std::cout << loadedTextures[j].path.data() << "\n";
            id = loadedTextures[j].id;
            return;
        }
    }

    std::vector<unsigned char*> imageDatas;
    
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D_ARRAY, id);
    
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
    
    GLenum format;
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
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
    Texture newTex;
    newTex.id = id;
    newTex.path = filePath;
    loadedTextures.push_back(newTex);
    stbi_set_flip_vertically_on_load(1);
    glBindTexture(GL_TEXTURE_2D_ARRAY,0);
}

static GLuint loadTexture(const char* filePath) {
 for (int j = 0; j < loadedTextures.size(); j++) {
       if (std::strcmp(loadedTextures[j].path.data(), filePath) == 0) {
           std::cout << loadedTextures[j].path.data() << "already loaded" << "\n";
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
       glTexImage2D(GL_TEXTURE_2D, 0,format, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, imageData);
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

    return model;
}

static void generateFramebuffer(Frame* frame, int x, int y) {
    glGenFramebuffers(1, &frame->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, frame->fbo);
    
    glGenTextures(1, &frame->ftexture);
    glBindTexture(GL_TEXTURE_2D, frame->ftexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frame->ftexture, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
   glGenRenderbuffers(1, &frame->frbo);
    glBindRenderbuffer(GL_RENDERBUFFER, frame->frbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,x, y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frame->frbo);
    glBindRenderbuffer(GL_RENDERBUFFER,0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frame->frbo);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glGenVertexArrays(1, &frame->fvao);
    glBindVertexArray(frame->fvao);
    
    glGenBuffers(1, &frame->fvbo);
    glBindBuffer(GL_ARRAY_BUFFER, frame->fvbo);
    float screenquad[24] =
    {   -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    glBufferData(GL_ARRAY_BUFFER, 24*sizeof(float), &screenquad[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    
    frame->shader.init("Shaders/FBufferVShader.vs", "Shaders/FBufferFShader.fs");
}

static void generateFramebuffer2Color(DoubleFrame* frame, int x, int y) {
    glGenFramebuffers(1, &frame->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, frame->fbo);
    
    glGenTextures(1, &frame->ftexture0);
    glBindTexture(GL_TEXTURE_2D, frame->ftexture0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frame->ftexture0, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glGenTextures(1, &frame->ftexture1);
    glBindTexture(GL_TEXTURE_2D, frame->ftexture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, frame->ftexture1, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
    
   glGenRenderbuffers(1, &frame->frbo);
    glBindRenderbuffer(GL_RENDERBUFFER, frame->frbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,x, y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frame->frbo);
    glBindRenderbuffer(GL_RENDERBUFFER,0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frame->frbo);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glGenVertexArrays(1, &frame->fvao);
    glBindVertexArray(frame->fvao);
    
    glGenBuffers(1, &frame->fvbo);
    glBindBuffer(GL_ARRAY_BUFFER, frame->fvbo);
    float screenquad[24] =
    {   -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    glBufferData(GL_ARRAY_BUFFER, 24*sizeof(float), &screenquad[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    
    frame->shader.init("Shaders/FBufferVShader.vs", "Shaders/FBufferFShader.fs");
}

static void generateFramebuffer(Frame* frame, GLuint* ftexture_, int x, int y) {
    glGenFramebuffers(1, &frame->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, frame->fbo);
    
    frame->ftexture = *ftexture_;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *ftexture_, 0);
    
   glGenRenderbuffers(1, &frame->frbo);
    glBindRenderbuffer(GL_RENDERBUFFER, frame->frbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, x, y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frame->frbo);
    glBindRenderbuffer(GL_RENDERBUFFER,0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frame->frbo);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glGenVertexArrays(1, &frame->fvao);
    glBindVertexArray(frame->fvao);
    
    glGenBuffers(1, &frame->fvbo);
    glBindBuffer(GL_ARRAY_BUFFER, frame->fvbo);
    float screenquad[24] =
    {   -0.10f,  0.10f,  0.0f, 1.0f,
        -0.10f, -0.10f,  0.0f, 0.0f,
         0.10f, -0.10f,  1.0f, 0.0f,

        -0.10f,  0.10f,  0.0f, 1.0f,
         0.10f, -0.10f,  1.0f, 0.0f,
         0.10f,  0.10f,  1.0f, 1.0f
    };

    glBufferData(GL_ARRAY_BUFFER, 24*sizeof(float), &screenquad[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    
    frame->shader.init("Shaders/GIFFrameVShader.vs", "Shaders/GIFFrameFShader.fs");
}

/**static std::vector<Mesh> processNode(aiNode* node, const aiScene* scene);

Mesh processMesh(aiMesh* mesh, const aiScene* scene);

std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type_, std::string typeName);**/
#endif /* AssetManager_hpp */

