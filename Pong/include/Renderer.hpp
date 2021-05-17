//
//  Renderer.hpp
//  Pong
//
//  Created by Joseph Gu on 5/3/21.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <vector>
#include "Shader.hpp"
#include "World.hpp"
#include "Camera.hpp"
#include "stb_image.h"
#include "Mesh.hpp"

class Renderer {
    GLuint VBO, VAO, EBO, mVBO, mVAO, mEBO, sVBO, sVAO;
    GLuint texture = 0;
    GLuint skyTexture = 0;
    int numActorTriangles;
    World* world;
    Shader* actorShader;
    Shader* mapShader;
    Shader* skyShader;
    Camera* camera;
    GLFWwindow* window;
    glm::mat4 modelMat;
    glm::mat4 viewMat;
    glm::mat4 projMat;
    void loadModelMatrix(int n); // finds the matrices for nth actor in world's actor vector
public:
    Renderer();
    ~Renderer();
    void setWorld(World* world_);
    void setCamera(Camera* camera_);
    void loadActorData();
    void loadMapData();
    void loadSkyBoxData();
    void render();
};
#endif /* Renderer_hpp */

