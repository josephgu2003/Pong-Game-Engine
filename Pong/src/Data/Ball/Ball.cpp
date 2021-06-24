//
//  Ball.cpp
//  Pong
//
//  Created by Joseph Gu on 5/1/21.
//

#include "Ball.hpp"
#include "AssetManager.hpp"
#include "FallingLetters.hpp"
#include "Speech.hpp"
#include "BossBehaviorComponent.hpp"
#include "PhysicsComponent.hpp"


Ball::Ball() {
    std::cout << "oof2";
        posVec.x = 0.0f;
        posVec.y = 0.0f;
        posVec.z = 0.0f;
        dirVec = glm::vec3(0.0f, 0.0f, -1.0f);
        velVec = glm::vec3(0.0f, 0.0f, 0.0f);
        rightVec = glm::cross(dirVec,glm::vec3(0,1,0));
    PhysicsComponent* pC = new PhysicsComponent(false);
    components.push_back(pC);
    state = STATE_FLYING;
     /**   float verticesCopy [] = {
            -0.125f, -0.125f, -0.125f,
            -0.125f, -0.125f, 0.125f,
            0.125f, -0.125f, 0.125f,
            0.125f, -0.125f, -0.125f,
            -0.125f, 0.125f, -0.125f,
            -0.125f, 0.125f, 0.125f,
            0.125f, 0.125f, 0.125f,
            0.125f, 0.125f, -0.125f
        };
        memcpy(vertices, verticesCopy, sizeof(verticesCopy));
        GLuint indicesCopy [] = {
            0, 1, 2,
            2, 3, 0,
            4, 5, 6,
            6, 7, 4,
            0, 1, 5,
            5, 4, 0,
            3, 2, 6,
            6, 7, 3,
            1, 2, 5,
            5, 6, 2,
            0, 3, 7,
            7, 4, 0
        };
    memcpy(indices, indicesCopy, sizeof(indicesCopy));**/
}

void Ball::init() {
    model = loadModels("Resources/Models/journey5.obj");
   // model = loadModels(MOD_JUGGERNAUT);
    shader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs");
    
    extern GLuint uboViewProj;
    glBindBuffer(GL_UNIFORM_BUFFER, uboViewProj);
    GLuint viewproj  = glGetUniformBlockIndex(shader->ID, "ViewProj");
    glUniformBlockBinding(shader->ID, glGetUniformBlockIndex(shader->ID, "ViewProj"), 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboViewProj);
    
    
    extern GLuint uboLights;
    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
    GLuint lights  = glGetUniformBlockIndex(shader->ID, "Lights");
    glUniformBlockBinding(shader->ID, glGetUniformBlockIndex(shader->ID, "Lights"), 1);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboLights);
 //   GLuint lights  = glGetUniformBlockIndex(shader.ID, "Lights");
   // glUniformBlockBinding(shader.ID, glGetUniformBlockIndex(shader.ID, "Lights"), 1);
    std::vector<GLuint> newMaps = {loadTexture(TEX_INKPAPER)};
    for (int i = 0; i<model->getMeshes()->size(); i ++) {
        model->setMeshTexture(i, DIFFUSE, newMaps);
    }
    BossBehaviorComponent* component = new BossBehaviorComponent();
    components.push_back(component);
}

void Ball::tick() {
    Actor::tick();
}
