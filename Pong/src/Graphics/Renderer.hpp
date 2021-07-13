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
#include "Camera.hpp"
#include "Mesh.hpp"
#include <string>
#include "Batch.hpp"
#include "AssetManager.hpp"

// draws the world
struct screentext {
    float duration;
    std::string text;
    std::map<char, Character> Characters;
    std::vector<float> textPosArray;
};

class Renderer {
private:
    screentext screenText = {0, ""};
    GLuint funtex1, funtex2;
    GLuint VBO, VAO, EBO, mVBO, mVAO, mEBO, sVBO, sVAO, pVAO, pVBO, pEBO, tVBO, tVAO, qVBO, qVAO, qEBO;
    Batch<SimpleVertex> pointParticles;
    Batch<SimpleVertex> quadParticles;
    Batch<TBNVertex> models;
    Batch
    float screenquad[24] =
    {   -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    Frame frame;
    GLuint texture = 0;
    GLuint texture2 = 0;
    GLuint skyTexture = 0;
    GLuint gradient = 0;
    GLuint noise = 0;

    Shader* skyShader;
    Shader* frameShader;
    Shader* textShader;
    Shader* sketchShader;
    
    glm::mat4 modelMat;
    glm::mat4 viewMat;
    glm::mat4 projMat;
    glm::mat4 viewMat2;
    glm::mat4 projMat2;
    
    Camera* camera;
    World* world;
    float lighting;
    

    void renderMap();
    void renderActors();
    void renderParticles();
    void renderQuads();
    void renderUI();
    
    void updateUniformBlocks();
    void updateViewProj();
    void updateLights();
    void updateCamPos();
    void updateUniformStopWatch();
    float timeT;
    
    void updateParticleBatches();
public:
    void renderSky();
    Renderer();
    ~Renderer();
    void setWorld(World* world_);
    void setCamera(Camera* camera_);
    void loadActorData();
    void loadMapData();
    void loadSkyBoxData();
    void loadParticleData();
    void loadQuadData();
    void loadTextData();
    void render();
    void print(std::string string);
    void renderText();
    void checkForUpdates();
};
#endif /* Renderer_hpp */

