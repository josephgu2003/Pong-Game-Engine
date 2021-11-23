//
//  Renderer.hpp
//  Pong
//
//  Created by Joseph Gu on 5/3/21.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#define RENDER_DISTANCE 200.0

#include <glm/glm.hpp>
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <vector>
#include "Shader.hpp"
#include "World.hpp"
#include <string>
#include <memory>

extern GLuint uboViewProj;
extern GLuint uboLights; 
extern GLuint uboStopWatch;
extern GLuint uboDistanceFog;

enum Uniblock {
    ViewProj, Lights, StopWatch, DistanceFog
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

class GraphicsObject;

class Renderer {
private:
    float scale = 0;
    float fieldOfView, frustrumNear, frustrumFar;
    GLuint mVBO, mVAO, mEBO, sVBO, sVAO, qVBO, qVAO, qEBO;
    
    DoubleFrame frame2C;
    Frame frame0;
    Frame frame1;
    
    Texture texture;
    Texture skyTexture;
    Texture gradient;
    Texture noise;

    Shader* skyShader; 
    Shader* blurShader;
    Shader* frameShader;
    Shader* uiShader;
    
    glm::mat4 viewMat;
    glm::mat4 projMat;
    
    Camera* camera = NULL;
    World* world = NULL;
    GLFWwindow* window = NULL; 
    float lighting;
    
    void updateDistanceFog();
    void updateViewProj();
    void updateCamPos();
    void updateLights();
    void updateUniformStopWatch();
    float timeT;
    void bindTextures(Shader* shader, TextureMaps& map);
    void resizeViewPort();
public:
    static void bindShaderUniblock(Shader* shader, Uniblock block);
    void updateAllUniblocks();
    void renderUI(uiPiece* uip);
    void renderSky(); 
    Renderer();
    ~Renderer();
    void setWorld(World* world_);
    void setCamera(Camera* camera_);
    void loadSkyBoxData();
    void renderInitial();
    void renderFinal();
    void renderText(uiText* text); 
    void checkForUpdates();
    void renderActor(GraphicsObject* r);
    void renderParticles(GraphicsObject* r, int instanceCount);
    void renderTerrain(GraphicsObject* r);
    void renderFoliage(GraphicsObject* r);
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
