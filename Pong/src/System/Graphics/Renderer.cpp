//
//  Renderer.cpp
//  Pong
//
//  Created by Joseph Gu on 5/3/21.
//

#include "Renderer.hpp"
#include "stb_image.h"
#include <iostream>
#include "AssetManager.hpp"
#include <map>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

GLuint uboViewProj;
GLuint uboLights;

Renderer::Renderer() {
    actorShader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs"); // make own function later
    mapShader = new Shader("Shaders/MapVertexShader.vs", "Shaders/MapFragmentShader.fs");
    skyShader = new Shader("Shaders/SkyVertexShader.vs", "Shaders/SkyFragmentShader.fs");
    particleShader = new Shader("Shaders/ParticleVertexShader.vs", "Shaders/ParticleFragmentShader.fs");
    glyphParticleShader = new Shader("Shaders/GlyphParticleVShader.vs", "Shaders/GlyphParticleFShader.fs");
    frameShader = new Shader("Shaders/FBufferVShader.vs", "Shaders/FBufferFShader.fs");
    textShader = new Shader("Shaders/TextVShader.vs", "Shaders/TextFShader.fs");
    sketchShader = new Shader("Shaders/SketchVShader.vs", "Shaders/SketchFShader.fs");
    
    shaders.push_back(actorShader);
    shaders.push_back(mapShader);
    shaders.push_back(skyShader);
    shaders.push_back(particleShader);
    shaders.push_back(glyphParticleShader);
    shaders.push_back(textShader);
    shaders.push_back(sketchShader);
    
    modelMat = glm::mat4(1);
    viewMat = glm::mat4(1);
    projMat = glm::perspective(glm::radians(50.0f), 1000.0f/800.0f, 0.01f, 100.0f);
    projMat2 = glm::perspective(glm::radians(50.0f), 1000.0f/800.0f, 0.01f, 100.0f);
    time = 0;
    gradient = loadTexture(TEX_GRADIENT);
    funtex2 = loadTexture("Resources/Models/textures/lambert1_baseColor.png");
    noise = loadTexture("Resources/Utility/noise.png");
   // texture = loadTexture("Resources/snowy-mountain-terrain/textures/SM_DiffJPG.jpg");
    texture2 = loadTexture("Resources/Map/mountains-free/source/ThickPlane_1-AO_u0_v0.jpg");
    texture = loadTexture(TEX_EMPTY);
    
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    glGenTextures(1, &ftexture);
    glBindTexture(GL_TEXTURE_2D, ftexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2000, 1600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ftexture, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
   glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 2000, 1600);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    glBindRenderbuffer(GL_RENDERBUFFER,0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glGenVertexArrays(1, &fvao);
    glBindVertexArray(fvao);
    
    glGenBuffers(1, &fvbo);
    glBindBuffer(GL_ARRAY_BUFFER, fvbo);

    glBufferData(GL_ARRAY_BUFFER, 24*sizeof(float), &screenquad[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

}

Renderer::~Renderer() {
    
}

void Renderer::setWorld(World *world_) {
    world = world_;
}

void Renderer::setCamera(Camera *camera_) {
    camera = camera_;

    glBindVertexArray(VAO);
    glGenBuffers(1, &uboViewProj);
    glBindBuffer(GL_UNIFORM_BUFFER, uboViewProj);
    glBufferData(GL_UNIFORM_BUFFER, 128, NULL, GL_STATIC_DRAW); // allocate 152 bytes of memory
    
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboViewProj);

    glGenBuffers(1, &uboLights);
    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
    glBufferData(GL_UNIFORM_BUFFER, 9*sizeof(glm::vec4), NULL, GL_STATIC_DRAW); // allocate 152 bytes of memory
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboLights);
    
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindVertexArray(0);
    updateUniformBlocks();
}

void Renderer::updateUniformBlocks() {

    updateViewProj();
    updateLights();
    updateViewPos();

}

void Renderer::updateViewProj() {
    glBindBuffer(GL_UNIFORM_BUFFER, uboViewProj);
    viewMat = glm::lookAt(camera->posVec,camera->dirVec+camera->posVec, glm::vec3(0.0,1.0,0.0));
    
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, glm::value_ptr(viewMat));
    glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, glm::value_ptr(projMat));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void Renderer::updateLights() {
    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
     
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 16, glm::value_ptr(glm::vec4(0.1,0.1,0.1,0)));
     glBufferSubData(GL_UNIFORM_BUFFER, 16, 16, glm::value_ptr(glm::vec4(0.2,0.2,0.2,0)));
     glBufferSubData(GL_UNIFORM_BUFFER,  2*16,16, glm::value_ptr(glm::vec4(0.2,0.2,0.2,0)));
     glBufferSubData(GL_UNIFORM_BUFFER, 3*16, 16, glm::value_ptr(glm::vec4(0.2,0.2,0.2,0)));
     
     glBufferSubData(GL_UNIFORM_BUFFER, 4*16,16,glm::value_ptr(glm::vec4(0,-1,-1,0)));
     glBufferSubData(GL_UNIFORM_BUFFER, 5*16, 16, glm::value_ptr(glm::vec4(0.1,0.1,0.1,0)));
     glBufferSubData(GL_UNIFORM_BUFFER, 6*16,16, glm::value_ptr(glm::vec4(0.2,0.2,0.2,0)));
     glBufferSubData(GL_UNIFORM_BUFFER, 7*16, 16, glm::value_ptr(glm::vec4(1.0,1.0,1.0,0)));
     glBufferSubData(GL_UNIFORM_BUFFER, 8*16, 16, glm::value_ptr(glm::vec4(camera->posVec,0)));
     glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::updateViewPos() {
    glBindBuffer(GL_UNIFORM_BUFFER, uboViewProj);
    viewMat = glm::lookAt(camera->posVec,camera->dirVec+camera->posVec, glm::vec3(0.0,1.0,0.0));
    
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, glm::value_ptr(viewMat));
    glBindBuffer(GL_UNIFORM_BUFFER,0);
    
    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
     glBufferSubData(GL_UNIFORM_BUFFER, 8*16, 16, glm::value_ptr(glm::vec4(camera->posVec,0)));
     glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::loadActorData() {
    std::vector<Vertex> actorVertices;
    std::vector<GLuint> actorIndices;
    for (int i = 0; i<world->getActorsCount(); i++) { // iterate over actors
        std::vector<Mesh>* meshes = world->getNthActor(i)->model->getMeshes();

        for (int j = 0; j<meshes->size(); j++) { // iterate over meshes
            int marker = actorIndices.size();
            actorVertices.insert(actorVertices.end(), meshes->at(j).vertices.begin(), meshes->at(j).vertices.end());
            actorIndices.insert(actorIndices.end(), meshes->at(j).indices.begin(), meshes->at(j).indices.end());
            for (marker; marker != actorIndices.size(); marker++) {
            actorIndices[marker] += actorVertices.size() - meshes->at(j).vertices.size();
            }
        }
    }
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, actorVertices.size() * sizeof(Vertex), actorVertices.data(), GL_STATIC_DRAW);
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, actorIndices.size() * sizeof(GLuint), actorIndices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
}

void Renderer::loadMapData() {
    std::vector<Vertex> mapVertices;
    std::vector<GLuint> mapIndices;
        std::vector<Mesh>* meshes = world->getMap().getMeshes();
        for (int j = 0; j<meshes->size(); j++) { // iterate over meshes
            int marker = mapIndices.size();
            mapVertices.insert(mapVertices.end(), meshes->at(j).vertices.begin(), meshes->at(j).vertices.end());
            mapIndices.insert(mapIndices.end(), meshes->at(j).indices.begin(), meshes->at(j).indices.end());
            for (marker; marker != mapIndices.size(); marker++) {
            mapIndices[marker] += mapVertices.size() - meshes->at(j).vertices.size();
            }
        }
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

  /**  glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, mapVertices.size() * sizeof(Vertex), mapVertices.data(), GL_STATIC_DRAW);
    
    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mapIndices.size() * sizeof(GLuint), mapIndices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);**/
    glGenVertexArrays(1, &mVAO);
      glBindVertexArray(mVAO);
      
      glGenBuffers(1, &mVBO);
      glBindBuffer(GL_ARRAY_BUFFER, mVBO);

      glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), world->getMapMesh()->mapFirstVertex, GL_STATIC_DRAW);
      
      glGenBuffers(1, &mEBO);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), world->getMapMesh()->mapFirstIndex, GL_STATIC_DRAW);
      
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  8*sizeof(float), (void*)(3*sizeof(float)));
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  8*sizeof(float), (void*)(6*sizeof(float)));
      glEnableVertexAttribArray(2);
      
    glBindVertexArray(0);
}

void Renderer::loadSkyBoxData() {
    int imageWidth, imageHeight, channels;
    unsigned char* imageData;
    glGenTextures(1, &skyTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture);
    std::vector<std::string>*skyFiles = world->getSkyTextureFiles();
    for (unsigned int i = 0; i < skyFiles->size(); i++) {
        imageData = stbi_load(skyFiles->at(i).c_str(), &imageWidth, &imageHeight, &channels, 0);
        if (imageData) {
            GLenum format;
                   if (channels == 1)
                       format = GL_RED;
                   else if (channels == 3)
                       format = GL_RGB;
                   else if (channels == 4)
                       format = GL_RGBA;
            glTexImage2D(
                         GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData
                         );
        } else {
            std::cout << "Failed to load sky box data \n";
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    stbi_image_free(imageData);
    
    glGenVertexArrays(1, &sVAO);
    glBindVertexArray(sVAO);
    
    glGenBuffers(1, &sVBO);
    glBindBuffer(GL_ARRAY_BUFFER, sVBO);

    glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), (world->getSkyVertices()), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Renderer::loadParticleData() {
    std::vector<ParticleEffect*>* particleData = world->getParticleEffects();
    std::vector<SimpleVertex>  particleVertices;
    std::vector<GLuint> particleIndices;
    std::vector<glm::vec3> particleInstances;
    
    SimpleVertex v1;
    SimpleVertex v2;
    SimpleVertex v3;
    SimpleVertex v4;
    v1.Pos = glm::vec3(0,0,0);
    v1.TexCoords = glm::vec2(0,0);
    v2.Pos = glm::vec3(1,0,0);
    v2.TexCoords = glm::vec2(1,0);
    v3.Pos = glm::vec3(1,1,0);
    v3.TexCoords = glm::vec2(1,1);
    v4.Pos = glm::vec3(0,1,0);
    v4.TexCoords = glm::vec2(0,1);
    std::vector<SimpleVertex> newVertices = {
        v1, v2, v3, v4
    };
    particleVertices.insert(particleVertices.end(), newVertices.begin(), newVertices.end());
    for (int j = 0; j<particleData->size(); j++) {
        for(int i = 0; i<particleData->at(j)->getNumParticles(); i++) {
        std::vector<GLuint> newIndices = {
            0, 1, 2,
            2, 3, 0
        };
        particleIndices.insert(particleIndices.end(), newIndices.begin(), newIndices.end());
        }
    }
  /**  for (int j = 0; j<particleData->size(); j++) {
        for (int i = 0; i < particleData->at(j)->getNumParticles(); i++) {
            particleInstances.push_back(particleData->at(j)->getNthParticle(i).posVec);
        }
    }**/
    glGenVertexArrays(1, &pVAO);
    glBindVertexArray(pVAO);
    
    glGenBuffers(1, &pVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pVBO);
    glBufferData(GL_ARRAY_BUFFER, particleVertices.size() * sizeof(SimpleVertex), particleVertices.data(), GL_STATIC_DRAW);
    
   glGenBuffers(1, &pEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, particleIndices.size() * sizeof(GLuint), particleIndices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);
  /**  glGenBuffers(1, &pinstaVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pinstaVBO);
    glBufferData(GL_ARRAY_BUFFER, particleInstances.size() * sizeof(glm::vec3), particleInstances.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(0));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 3);**/
    
    glBindVertexArray(0);
}

void Renderer::loadQuadData() {
    std::vector<Vertex> quadVertices;
    std::vector<GLuint> quadIndices;

    for (int i = 0; i<world->getQuads()->size(); i++) { // iterate over actors
        quadVertices.insert(quadVertices.end(), world->getQuads()->at(i)->vertices.begin(), world->getQuads()->at(i)->vertices.end());
        quadIndices.insert(quadIndices.end(), world->getQuads()->at(i)->indices.begin(), world->getQuads()->at(i)->indices.end());
    }
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glGenVertexArrays(1, &qVAO);
    glBindVertexArray(qVAO);
    
    glGenBuffers(1, &qVBO);
    glBindBuffer(GL_ARRAY_BUFFER, qVBO);

    glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * sizeof(Vertex), quadVertices.data(), GL_STATIC_DRAW);
    
    glGenBuffers(1, &qEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, qEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, quadIndices.size() * sizeof(GLuint), quadIndices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
}

void Renderer::checkForUpdates() {
    Updates updates = world->checkforUpdates();
    if(updates.actorUpdate == true) {
        world->updateCleared(ACTOR_UPDATE);
        loadActorData();
    }
    if(updates.particleUpdate == true) {
        world->updateCleared(PARTICLE_UPDATE);
        loadParticleData();
    }
    if(updates.quadUpdate == true) {
        world->updateCleared(QUAD_UPDATE);
        loadQuadData();
    }
    if(updates.textUpdate == true) {
        world->updateCleared(TEXT_UPDATE);
        print(world->getActiveText());
    }
}

void Renderer::render() {
    checkForUpdates();
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.6,0.6,0.6,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (world->getWeather().brightnessExists) {
    lighting = world->getWeather().brightness;
    } else {
        lighting = 1.0f;
    }
    updateViewPos();
  //  renderSky();
    renderMap();
    renderActors();
    renderParticles();
    renderQuads();
    renderUI();
    
    if (screenText.duration > 0) {
        renderText();
    }
    time += (float)glfwGetTime();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    frameShader->use();
    glBindVertexArray(fvao);
    glDisable(GL_DEPTH_TEST);
    
    glActiveTexture(GL_TEXTURE0); // accomodate more trextures later
    glUniform1i(glGetUniformLocation(frameShader->ID, "fbotexture"), 0);
    glBindTexture(GL_TEXTURE_2D, ftexture);
    
    glActiveTexture(GL_TEXTURE1);
    glUniform1i(glGetUniformLocation(frameShader->ID, "noise"), 1);
    glBindTexture(GL_TEXTURE_2D, noise);
  glUniform1f(glGetUniformLocation(frameShader->ID, "time"), time);
    glUniform1i(glGetUniformLocation(frameShader->ID, "blur"), world->blur);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    screenText.duration -= glfwGetTime();
}

void Renderer::renderUI() {
    
}

void Renderer::loadTextData() {
    glGenVertexArrays(1, &tVAO);
    glBindVertexArray(tVAO);
    
    glGenBuffers(1, &tVBO);
    glBindBuffer(GL_ARRAY_BUFFER, tVBO);
    glBufferData(GL_ARRAY_BUFFER, screenText.textPosArray.size()*sizeof(float), screenText.textPosArray.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}



void Renderer::renderSky() {
    //sky
    glDisable(GL_BLEND);
    glDepthMask(GL_FALSE);
    skyShader->use();
    glUniform1f(glGetUniformLocation(skyShader->ID, "brightness"), lighting);
    viewMat = glm::lookAt(camera->posVec,camera->dirVec+camera->posVec, glm::vec3(0.0,1.0,0.0));
    glm::mat4 camViewMat = glm::mat4(glm::mat3(viewMat));
    glUniformMatrix4fv(glGetUniformLocation(skyShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(camViewMat));
    glUniformMatrix4fv(glGetUniformLocation(skyShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
    glBindVertexArray(sVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sVBO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}

void Renderer::renderQuads() {
    glDepthMask(GL_FALSE);
    if (world->getQuads()->size() > 0) {
    Quad* quad = world->getQuads()->at(0);
    sketchShader->use();
    glBindVertexArray(qVAO);
    glBindBuffer(GL_ARRAY_BUFFER, qVBO);
    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(Vertex)*quad->vertices.size(),quad->vertices.data());
    modelMat = glm::mat4(1.0f);
  //  modelMat = glm::translate(modelMat, quad->pos);
        glm::vec3 rotations = glm::vec3(glm::radians(quad->rotations.x),glm::radians(quad->rotations.y),glm::radians(quad->rotations.z));
    glm::quat MyQuaternion = glm::quat(rotations);

    glm::mat4 RotationMatrix = toMat4(MyQuaternion);

 //   modelMat = modelMat * RotationMatrix;

    glUniformMatrix4fv(glGetUniformLocation(sketchShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix4fv(glGetUniformLocation(sketchShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
    glUniformMatrix4fv(glGetUniformLocation(sketchShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));

        glUniform3fv(glGetUniformLocation(sketchShader->ID, "viewPos"), 1, glm::value_ptr(camera->posVec));
        glUniform3fv(glGetUniformLocation(sketchShader->ID, "light.pos"), 1, glm::value_ptr(glm::vec3(10,5,0)));
        glUniform3fv(glGetUniformLocation(sketchShader->ID, "light.ambient"), 1, glm::value_ptr(glm::vec3(0.1,0.1,0.1)));
        glUniform3fv(glGetUniformLocation(sketchShader->ID, "light.diffuse"), 1, glm::value_ptr(glm::vec3(0.5,0.5,0.5)));
        glUniform3fv(glGetUniformLocation(sketchShader->ID, "light.specular"), 1, glm::value_ptr(glm::vec3(1.0,1.0,1.0)));
       // glUniform1f(glGetUniformLocation(sketchShader->ID, "z_offsets"), billowing[0]);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(sketchShader->ID, "texture0"), 0);
        glBindTexture(GL_TEXTURE_2D, world->getQuads()->at(0)->texture);
        glDrawElements(GL_TRIANGLES, quad->indices.size(), GL_UNSIGNED_INT, (void*) 0);
    }
    modelMat = glm::mat4(1);
    glDepthMask(GL_TRUE);
}


void Renderer::renderMap() {
    //map
   // glActiveTexture(GL_TEXTURE0);
   // glBindTexture(GL_TEXTURE_2D, texture);
    mapShader->use();
    glUniform3fv(glGetUniformLocation(mapShader->ID, "viewPos"), 1, glm::value_ptr(camera->posVec));
    glUniform3fv(glGetUniformLocation(mapShader->ID, "dirLight.dir"), 1,  glm::value_ptr(glm::vec3(0,-1,-1)));
    glUniform3fv(glGetUniformLocation(mapShader->ID, "dirLight.ambient"), 1,
                 glm::value_ptr(glm::vec3(0.1,0.1,0.1)));
    glUniform3fv(glGetUniformLocation(mapShader->ID, "dirLight.diffuse"), 1, glm::value_ptr(glm::vec3(0.2,0.2,0.2)));
    glUniform3fv(glGetUniformLocation(mapShader->ID, "dirLight.specular"), 1, glm::value_ptr(glm::vec3(1,1,1)));
    
    modelMat = glm::mat4(1.0f);
   // modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(1,0,0));
    modelMat= glm::translate(modelMat, glm::vec3(0,-0.14,0));
    glUniformMatrix4fv(glGetUniformLocation(mapShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix4fv(glGetUniformLocation(mapShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
    glUniformMatrix4fv(glGetUniformLocation(mapShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(mVAO);
    glActiveTexture(GL_TEXTURE0); // accomodate more trextures later
        glUniform1i(glGetUniformLocation(mapShader->ID, "mapTexture"), 0);
      glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glUniform1i(glGetUniformLocation(mapShader->ID, "texture1"), 1);
        glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1f(glGetUniformLocation(mapShader->ID, "time"), time);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::renderActors() {
    // actors
    glBindVertexArray(VAO);
    int indiceCount = 0;
    for (int i = 0; i<world->getActorsCount(); i++) {
    std::vector<Mesh>* meshes = world->getNthActor(i)->model->getMeshes();
        Shader* shader = world->getNthActor(i)->shader;
        shader->use();
        for (int j = 0; j < meshes->size(); j++) {
            
            GLuint id1;
            if (meshes->at(j).textures.size() > 0) {
                id1 = meshes->at(j).textures.at(0).id;
            } else {
                id1 = funtex2;            }
            GLuint id2;
            if (meshes->at(j).textures.size()>1) {
                id2 = meshes->at(j).textures.at(1).id;
            } else {
                id2 = texture;
            }
            
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(glGetUniformLocation(shader->ID, "texture0"), 0);
            glBindTexture(GL_TEXTURE_2D, id1);
            
          glActiveTexture(GL_TEXTURE1);
            glUniform1i(glGetUniformLocation(shader->ID, "texture1"), 1);
            glBindTexture(GL_TEXTURE_2D, id2);
            
            glActiveTexture(GL_TEXTURE0);

            glDrawElements(GL_TRIANGLES, meshes->at(j).indices.size(), GL_UNSIGNED_INT, (void*) indiceCount);
            
            indiceCount += (meshes->at(j).indices.size())*sizeof(GLuint);
        
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}

void Renderer::renderParticles() {
    //particles
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glBindVertexArray(pVAO);
    int indiceCount = 0;

    for (int i = 0; i < world->getParticleEffects()->size(); i++) {
         Shader &shaderRef = (world->getParticleEffects()->at(i)->shader);
        shaderRef.use();
        GLuint shader = shaderRef.ID;
        glActiveTexture(GL_TEXTURE1);
        glUniform1i(glGetUniformLocation(shader, "texture1"), 1);
        glBindTexture(GL_TEXTURE_2D, gradient);
 
         glActiveTexture(GL_TEXTURE0);
         glUniform1i(glGetUniformLocation(shader, "texture0"), 0);
        
        for (int j = 0; j < world->getParticleEffects()->at(i)->getNumParticles(); j++) {
             if(world->getParticleEffects()->at(i)->getNthParticle(j).duration > 0) {
                 glBindTexture(world->getParticleEffects()->at(i)->textureTarget, world->getParticleEffects()->at(i)->getNthParticle(j).texture);
                 modelMat = glm::mat4(1.0f);
                 modelMat = glm::translate(modelMat, world->getParticleEffects()->at(i)->getNthParticle(j).posVec);
                 modelMat = glm::rotate(modelMat, -glm::radians(-90+camera->getYaw()), glm::vec3(0,1,0));
                 modelMat = glm::rotate(modelMat, -glm::radians(camera->getPitch()), glm::vec3(1,0,0));
                 
                 glUniformMatrix4fv(glGetUniformLocation(shader, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
                 glUniform1f(glGetUniformLocation(shader, "duration"), world->getParticleEffects()->at(i)->getNthParticle(j).duration);
                 glUniform1f(glGetUniformLocation(shader, "size"), world->getParticleEffects()->at(i)->getSize());
                 glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) indiceCount);
                    indiceCount += 6*sizeof(GLuint);
                }
        }
    }
    glActiveTexture(0);
    glDepthMask(GL_TRUE);
}

void Renderer::print(std::string string) {
    screenText.duration = 0.5;
    screenText.text = string;
    if (screenText.Characters.size() == 0){
    loadGlyphs("Resources/Glyphs/times.ttf", screenText.Characters);
    }
    screenText.textPosArray = {};
    std::string::const_iterator c;
    float x = -0.9;
    float y = -0.8;
    float scale = 0.0004;
      for (c = screenText.text.begin(); c != screenText.text.end(); c++)
      {
          Character ch = screenText.Characters[*c];

          float xpos = x + ch.bearing.x * scale;
          float ypos = y - (ch.size.y - ch.bearing.y) * scale;

          float w = ch.size.x * scale;
        
          float h = ch.size.y * scale;
          
          // update VBO for each character
          std::vector<float> vertices = {
               xpos,     ypos + h,   0.0f, 0.0f ,
              xpos,     ypos,       0.0f, 1.0f ,
               xpos + w, ypos,       1.0f, 1.0f ,
               xpos,     ypos + h,   0.0f, 0.0f ,
               xpos + w, ypos,       1.0f, 1.0f ,
               xpos + w, ypos + h,   1.0f, 0.0f
          };
          screenText.textPosArray.insert(screenText.textPosArray.end(), vertices.begin(), vertices.end());
          x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
      }
    loadTextData();
}

void Renderer::renderText() {
    textShader->use();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(tVAO);
    int verticeCount = 0;
    for (int i = 0; i < screenText.text.size(); i++) {
    glBindTexture(GL_TEXTURE_2D, screenText.Characters[screenText.text[i]].id);
    glDrawArrays(GL_TRIANGLES, verticeCount, 6);
    verticeCount += 6;
    }
}

/** int imageWidth, imageHeight, channels;   OLD FLAT MAP

 unsigned char* imageData = stbi_load("Resources/Map/ice.jpeg", &imageWidth, &imageHeight, &channels, 0);
 
 
 glGenTextures(1, &texture);
 glBindTexture(GL_TEXTURE_2D, texture);
 
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_viewMat_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
 
 if (imageData) {
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
     glGenerateMipmap(GL_TEXTURE_2D);
 } else {
     std::cout << "Failed to load image data \n";
 }
 stbi_image_free(imageData);
 
 mapMesh* mesh = world->getMapMesh();
 
 glGenVertexArrays(1, &mVAO);
 glBindVertexArray(mVAO);
 
 glGenBuffers(1, &mVBO);
 glBindBuffer(GL_ARRAY_BUFFER, mVBO);

 glBufferData(GL_ARRAY_BUFFER, mesh->mapVertexCount * sizeof(float), mesh->mapFirstVertex, GL_STATIC_DRAW);
 glGenBuffers(1, &mEBO);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->mapIndexCount *sizeof(GLuint), mesh->mapFirstIndex, GL_STATIC_DRAW);
 
 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
 glEnableVertexAttribArray(0);
 glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
 glEnableVertexAttribArray(1);
 glBindVertexArray(0);**/

/**  for (int i = 0; i < world->getParticleEffects()->size(); i++) { PARTICLE INSTANCING
 
      for (int j = 0; j < world->getParticleEffects()->at(i)->getNumParticles(); j++) {
          pPositions[j]=(world->getParticleEffects()->at(i)->getNthParticle(j).posVec);
      }
      shaders.at(world->getParticleEffects()->at(i)->shader)->use();
      GLuint shader = shaders.at(world->getParticleEffects()->at(i)->shader)->ID;
      glUniformMatrix4fv(glGetUniformLocation(shader, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
      glUniformMatrix4fv(glGetUniformLocation(shader, "projMat"), 1, GL_FALSE,
                  glm::value_ptr(projMat));
      glActiveTexture(GL_TEXTURE0);
      glUniform1i(glGetUniformLocation(actorShader->ID, "texture0"), 0);
      glBindTexture(GL_TEXTURE_2D, world->getParticleEffects()->at(i)->texture);
  /**    for (int j = 0; j < world->getParticleEffects()->at(i)->getNumParticles(); j++) {
          if(world->getParticleEffects()->at(i)->getNthParticle(j).duration > 0) {
              //
      glBindTexture(GL_TEXTURE_2D, world->getParticleEffects()->at(i)->getNthParticle(j).texture);
              // world->getParticleEffects()->at(i).getNthParticle(j).texture
      modelMat = glm::mat4(1.0f);
      modelMat = glm::translate(modelMat, world->getParticleEffects()->at(i)->getNthParticle(j).posVec);
      modelMat = glm::rotate(modelMat, -glm::radians(-90+camera->getYaw()), glm::vec3(0,1,0));
      glUniformMatrix4fv(glGetUniformLocation(shader, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
      glUniform1f(glGetUniformLocation(shader, "duration"), world->getParticleEffects()->at(i)->getNthParticle(j).duration);
      glUniform1f(glGetUniformLocation(shader, "size"), world->getParticleEffects()->at(i)->getSize());
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) indiceCount);
              indiceCount += 6*sizeof(GLuint);
      }*
      glUniform1f(glGetUniformLocation(shader, "size"), world->getParticleEffects()->at(i)->getSize());
      glUniform3fv(glGetUniformLocation(shader, "positions"), 1000, glm::value_ptr(pPositions[0]));
      glDrawArraysInstanced(GL_TRIANGLES, 0,6, world->getParticleEffects()->at(i)->getNumParticles());
      glActiveTexture(0);**/
