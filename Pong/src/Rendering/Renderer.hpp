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
#include "uiLayout.hpp"
#include "Batch.hpp"

extern GLuint uboViewProj;
extern GLuint uboLights; 
extern GLuint uboStopWatch;


enum Uniblock {
    ViewProj, Lights, StopWatch
};

// draws the world
struct screentext {
    float duration;
    std::string text;
    std::map<char, Character> Characters;
    std::vector<float> textPosArray;
};

class Renderer {
private:
    uiLayout uilayout;
    
    screentext screenText;
    GLuint funtex1, funtex2;
    GLuint VBO, VAO, EBO, mVBO, mVAO, mEBO, sVBO, sVAO, pVAO, pVBO, pEBO, tVBO, tVAO, qVBO, qVAO, qEBO;
    Batch pointParticles;
    Batch quadParticles;

    Batch uiStuff;
    std::vector<ParticleEffect*> loadedParticles;
    
    float screenquad[24] =
    {   -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    DoubleFrame frame2C;
    Frame frame0;
    Frame frame1;
    Texture texture ;
    Texture texture2 ;
    Texture skyTexture ;
    Texture gradient ;
    Texture noise ;

    Shader* skyShader;
    Shader* textShader;
    Shader* sketchShader;
    Shader* blurShader;
    Shader* frameShader;
    Shader* uiShader;
    
    glm::mat4 modelMat;
    glm::mat4 viewMat;
    glm::mat4 projMat;
    
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
    void updateCamPos();
    void updateUniformStopWatch();
    float timeT;

public:
    static void bindShaderUniblock(Shader* shader, Uniblock block);
    float exposure;
    void incExposure(float delta);
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
    void loadQuadData();
    void loadTextData();
    void render();
    void print(std::string string);
    void renderText();
    void checkForUpdates();
};
#endif /* Renderer_hpp */

