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
#define TEX_VORONOI "Resources/Utility/voronoi.png"

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
#include "json.hpp"
#include <memory>

//loads textures and assets


inline std::vector<std::unique_ptr<Model>> loadedModels;
inline std::vector<Shader> loadedShaders;
inline nlohmann::json dialogues;

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
};

struct DoubleFrame {
    GLuint fbo;
    GLuint fvao;
    GLuint fvbo;
    GLuint frbo;
    GLuint ftexture0;
    GLuint ftexture1;
};

struct DialogueTree;

class Dialogue;

namespace AssetManager {
inline std::vector<Texture> loadedTextures;

enum Uniblock {
    ViewProj, Lights, StopWatch
};
void bindShaderUniblock(Shader* shader, Uniblock block);
void buildTree(DialogueTree*& tree, int i, int branchID);

void loadDialogue(Dialogue* dialogue, int id);
void loadNullTexture(int x, int y, GLuint* texture, GLenum format);

int loadGlyphs(const char* filePath, std::map<char, Character>& Characters);

void load3DTexture(const char* filePath, Texture* texture);

void loadTexture(const char* filePath, Texture* texture, bool srgb);

};


static Model* loadModels(const char* filePath) {
    for (int i = 0; i < loadedModels.size(); i++) {
        std::string s(filePath);
        if (s == loadedModels.at(i)->getFilepath()) {
            Model* model = new Model();
            *model = *loadedModels.at(i).get();
            return model;
        }
    }
    
    std::unique_ptr<Model> model = std::make_unique<Model>(filePath);
    Model* model2 = new Model();
    *model2 = *model.get();
    loadedModels.push_back(std::move(model));
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

    return model2;
}

static void generateFramebuffer(Frame* frame, GLenum internalFormat, int x, int y) {
    glGenFramebuffers(1, &frame->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, frame->fbo);
    
    glGenTextures(1, &frame->ftexture);
    glBindTexture(GL_TEXTURE_2D, frame->ftexture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    

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
    
}

static void generateFramebuffer2Color(DoubleFrame* frame, int x, int y) {
    glGenFramebuffers(1, &frame->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, frame->fbo);
    
    glGenTextures(1, &frame->ftexture0);
    glBindTexture(GL_TEXTURE_2D, frame->ftexture0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frame->ftexture0, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glGenTextures(1, &frame->ftexture1);
    glBindTexture(GL_TEXTURE_2D, frame->ftexture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
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
    
}

/**static std::vector<Mesh> processNode(aiNode* node, const aiScene* scene);

Mesh processMesh(aiMesh* mesh, const aiScene* scene);

std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type_, std::string typeName);**/
#endif /* AssetManager_hpp */

