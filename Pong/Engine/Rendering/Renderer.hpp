//
//  Renderer.hpp
//  Pong
//
//  Created by Joseph Gu on 5/3/21.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#define WINDOW_WIDTH 1000.0
#define WINDOW_HEIGHT 650.0

#define RENDER_DISTANCE 200.0

#include <glm/glm.hpp>
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <vector>
#include "Shader.hpp"
#include <string>
#include "AssetManager.hpp"
#include <memory>
#include "GraphicsObject.hpp"


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
class PointLight;
class DirectionalLight;

class Renderer {
private:
    float scale = 0;
    float fieldOfView, frustrumNear, frustrumFar;
    GLuint mVBO, mVAO, mEBO, qVBO, qVAO, qEBO;
    
    DoubleFrame frame2C;
    Frame frame0;
    Frame frame1;
    
    Texture empty;
    Texture skyTexture;
    Texture gradient;
    Texture noise;
    Texture voronoi;

    Shader* blurShader;
    Shader* frameShader;
    
    glm::mat4 viewMat;
    glm::mat4 projMat;
     
    Camera* camera = NULL;
    GLFWwindow* window = NULL; 
    float lighting;
    
    void updateViewProj();
    void updateCamPos();
    void updateUniformStopWatch();
    float timeT;
    void bindTextures(Shader* shader, Material& map);
    void resizeViewPort();
    inline void bindGraphicsObject(GraphicsObject* go) {
        Shader* s = go->getShader();
        s->use();
        go->bind();
        bindTextures(s, go->getTextureMap());
    }  
public:
    static void bindShaderUniblocks(Shader* shader);
    void updateLights(const DirectionalLight& dl); // could make these more general with template containing struct DirectionalLight or DistanceFog and type erasure
    void updateLight(const PointLight& pl);
    void updateDistanceFog(float fogDensity, float fogGradient, glm::vec3 fogColor);
    static void bindShaderUniblock(Shader* shader, Uniblock block);
    void updateAllUniblocks();
    void renderSky(GraphicsObject* r); 
    Renderer();
    ~Renderer();
    void setCamera(Camera* camera_);
    void loadSkyBoxData();
    void renderInitial();
    void renderFinal();
    void renderParticles(GraphicsObject* r);
    void renderTerrain(GraphicsObject* r);
    void renderFoliage(GraphicsObject* r);
    void renderGeneric(GraphicsObject* go);
    void renderAdditiveBlend(GraphicsObject* r); 
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
