//
//  AssetManager.hpp
//  Pong
//
//  Created by Joseph Gu on 5/18/21.
//

#ifndef AssetManager_hpp
#define AssetManager_hpp

#include "Filepaths.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Model.hpp"
#include "stb_image.h"
#include <stdio.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include "Shader.hpp"
#include <memory>

//loads textures and assets

struct Character {
    GLuint id;
    glm::ivec2   size;       // Size of glyph
    glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
    GLuint advance; // advancing to next glph?
    glm::vec2 texCoords;
};

struct Frame {
    int width;
    int height;
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
    int width;
    int height;
};

struct Texture {
    GLuint id = -1;
    std::string type;
    std::string path;
    glm::vec3 dimensions;
    GLenum textureTarget;
};

struct TextureMaps {
    Texture diffuse;
    Texture specular;
    Texture normMap;
    Texture noise;
    Texture gradient;
    Texture voronoi;
};
 
class AssetManager {
private:
    static std::vector<Texture> loadedTextures;
    static std::vector<std::pair<Texture, std::map<char, Character>>> loadedGlyphs;
  //  static std::vector<std::unique_ptr<Model>> loadedModels;
    static std::vector<Shader> loadedShaders;
public:
    static void loadNullTexture(int x, int y, GLuint* texture, GLenum format);

    static int loadGlyphs(const char* filePath, std::map<char, Character>& Characters, TextureMaps& map);

    static void load3DTexture(const char* filePath, Texture* texture);

    static void loadTexture(const char* filePath, Texture* texture, bool srgb);

   // static void loadModel(const char* filePath, Model*& model);

   // static void loadModel(const char* filePath, Model*& model, AnimComponent* anim);

    static void generateFramebuffer(Frame* frame, GLenum internalFormat, int x, int y);

    static void generateFramebuffer2Color(DoubleFrame* frame, int x, int y);

    static void generateFramebuffer(Frame* frame, GLuint* ftexture_, int x, int y);

};



  
#endif /* AssetManager_hpp */ 
 
