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
#define MOD_WOLF "Resources/Models/wolf/wolf.fbx" 
#define MOD_BIRD "Resources/Models/bird/fly.fbx" 
#define MOD_VAMP "Resources/Models/Vampire/dancing_vampire.dae"

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

void buildTree(DialogueTree*& tree, int i, int branchID);

void loadDialogue(Dialogue* dialogue, int id);
void loadNullTexture(int x, int y, GLuint* texture, GLenum format);

int loadGlyphs(const char* filePath, std::map<char, Character>& Characters);

void load3DTexture(const char* filePath, Texture* texture);

void loadTexture(const char* filePath, Texture* texture, bool srgb);

void loadModel(const char* filePath, Model*& model);


void loadModel(const char* filePath, Model*& model, AnimComponent* anim);

void generateFramebuffer(Frame* frame, GLenum internalFormat, int x, int y);


void generateFramebuffer2Color(DoubleFrame* frame, int x, int y);

void generateFramebuffer(Frame* frame, GLuint* ftexture_, int x, int y);

};






/**static std::vector<Mesh> processNode(aiNode* node, const aiScene* scene);

Mesh processMesh(aiMesh* mesh, const aiScene* scene);

std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type_, std::string typeName);**/
#endif /* AssetManager_hpp */

