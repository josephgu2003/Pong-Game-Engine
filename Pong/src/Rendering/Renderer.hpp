//
//  Renderer.hpp
//  Pong
//
//  Created by Joseph Gu on 5/3/21.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include <glm/glm.hpp>
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <vector>
#include "Shader.hpp"
#include "World.hpp"
#include <string>
#include "uiLayout.hpp"
#include "Batch.hpp"
#include <memory>

extern GLuint uboViewProj;
extern GLuint uboLights; 
extern GLuint uboStopWatch;


enum Uniblock {
    ViewProj, Lights, StopWatch
};

class Camera;
// draws the world
const static float screenquad[24] =
{   -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};

class Renderable;

class Renderer {
private:
    std::weak_ptr<uiLayout> ui;
    
    GLuint VBO, VAO, EBO, mVBO, mVAO, mEBO, sVBO, sVAO, tVBO, tVAO, qVBO, qVAO, qEBO;
    Batch<ParticleEffect> pointParticles;
    Batch<ParticleEffect> quadParticles;
    Batch<uiLayout> uiStuff;
    Batch<SoundText> soundTexts;
    
    std::vector<ParticleEffect*> loadedParticles;
    
    DoubleFrame frame2C;
    Frame frame0;
    Frame frame1;
    
    Texture texture;
    Texture skyTexture;
    Texture gradient;
    Texture noise;

    Shader* skyShader;
    Shader* textShader;
    Shader* sketchShader;
    Shader* blurShader;
    Shader* frameShader;
    Shader* uiShader;
    
    glm::mat4 viewMat;
    glm::mat4 projMat;
    
    Camera* camera = NULL;
    World* world = NULL;
    float lighting;
    
    void renderMap();
    void renderActors();
    void renderParticles();
    
    void updateUniformBlocks();
    void updateViewProj();
    void updateCamPos();
    void updateUniformStopWatch();
    float timeT;
    void bindTextures(Shader* shader, TextureMaps& map);
public:
    static void bindShaderUniblock(Shader* shader, Uniblock block);
    void renderUI(uiPiece* uip);
    void updateLights();
    void renderSky();
    Renderer();
    ~Renderer();
    void setWorld(World* world_);
    void setCamera(Camera* camera_);
    void loadActorData();
    void loadMapData();
    void loadSkyBoxData();
    void loadParticleData();
    void render();
    void render2();
    void renderText(uiText* text);
    void checkForUpdates();
    void setUI(const std::shared_ptr<uiLayout>& ui);
    void renderElements(Renderable* r);
};
#endif /* Renderer_hpp */
 
// renderable interface
// - Shader
// - VAO
// - VBO (vertex data)
// - EBO (indices)
// - instancing data?
// - If batch with others: share shader, share vao and vbo, vao and vbo become
// same as others, draw call becomes function pointer?

// batches for furniture and static basic objects and ui and text

// no vertexdata, keep some semblance of vertex data for dynamic draws
// fish -> option 1 : buffersubdata full vertices
// fish -> option 2 : make pos part of vertices in contiguous vbo memory
// option 3 :  instance draw - square is vertex set, instance data is position and texture
