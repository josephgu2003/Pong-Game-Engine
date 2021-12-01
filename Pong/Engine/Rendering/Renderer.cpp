//
//  Renderer.cpp
//  Pong
//
//  Created by Joseph Gu on 5/3/21.
//
#define WINDOW_WIDTH 1000.0
#define WINDOW_HEIGHT 650.0
#define DEFAULT_FRUSTRUM_NEAR 0.01f
#define DEFAULT_FRUSTRUM_FAR 200.0f
#define DEFAULT_FOV 60.0f
 
#include "Renderer.hpp" 
#include "stb_image.h" 
#include <iostream>
#include <map>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "DirectionalLight.hpp"
#include "AssetManager.hpp"
#include "Camera.hpp"
#include "GraphicsObject.hpp"

GLuint uboViewProj; 
GLuint uboLights;
GLuint uboStopWatch;
GLuint uboDistanceFog;

void Renderer::bindShaderUniblock(Shader* shader, Uniblock block) {
    if (block == ViewProj) {
        glUniformBlockBinding(shader->ID, glGetUniformBlockIndex(shader->ID, "ViewProj"), 0);
    }
 
    if (block == Lights) {
        glUniformBlockBinding(shader->ID, glGetUniformBlockIndex(shader->ID, "Lights"), 1);
    }
    
    if (block == StopWatch) {
        glUniformBlockBinding(shader->ID, glGetUniformBlockIndex(shader->ID, "StopWatch"), 2);
    }
    
    if (block == DistanceFog) {
        glUniformBlockBinding(shader->ID, glGetUniformBlockIndex(shader->ID, "DistanceFog"), 3);
    }
}

Renderer::Renderer() {
    fieldOfView = DEFAULT_FOV;
    frustrumNear = DEFAULT_FRUSTRUM_NEAR;
    frustrumFar = DEFAULT_FRUSTRUM_FAR;
    
    window = glfwGetCurrentContext();
    skyShader = new Shader("Shaders/SkyVertexShader.vs", "Shaders/SkyFragmentShader.fs");
    blurShader = new Shader("Shaders/UI.vs","Shaders/BloomFShader.fs");
    frameShader = new Shader("Shaders/UI.vs", "Shaders/FBufferFShader.fs");
    
    viewMat = glm::mat4(1);
    float ratio = (WINDOW_WIDTH)/(WINDOW_HEIGHT);
    projMat = glm::perspective(glm::radians(fieldOfView), ratio, frustrumNear, frustrumFar); 
    timeT = 0; 
    AssetManager::loadTexture(TEX_GRADIENT, &gradient, false);
    AssetManager::loadTexture("Resources/Utility/noise.png", &noise, false);
    AssetManager::loadTexture(TEX_EMPTY, &empty, false);
    AssetManager::loadTexture(TEX_VORONOI, &voronoi, false);
       
    int width, height;  
    glfwGetFramebufferSize(window, &width, &height);
    scale = width  / float(WINDOW_WIDTH);
    printf("Window framebuffer is scaled by %f \n", scale);
    
    if (scale == 1.0) {
        scale = 2.0;
    }

    AssetManager::generateFramebuffer2Color(&frame2C, WINDOW_WIDTH*scale, WINDOW_HEIGHT*scale);
    AssetManager::generateFramebuffer(&frame0, GL_RGBA16F, 0.5 * WINDOW_WIDTH*scale, 0.5 * WINDOW_HEIGHT*scale);
    AssetManager::generateFramebuffer(&frame1, GL_RGBA16F,  0.5 * WINDOW_WIDTH*scale,
                                      0.5 * WINDOW_HEIGHT*scale);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_PROGRAM_POINT_SIZE);
    
    glGenBuffers(1, &uboViewProj);
    glBindBuffer(GL_UNIFORM_BUFFER, uboViewProj);
    glBufferData(GL_UNIFORM_BUFFER, 64, NULL, GL_DYNAMIC_DRAW); // allocate 152 bytes of memory
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboViewProj);

    glGenBuffers(1, &uboLights);
    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
    glBufferData(GL_UNIFORM_BUFFER, 9*sizeof(glm::vec4), NULL, GL_STATIC_DRAW); // allocate 152 bytes of memory
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboLights);
    

    glGenBuffers(1, &uboStopWatch);
    glBindBuffer(GL_UNIFORM_BUFFER, uboStopWatch); 
    glBufferData(GL_UNIFORM_BUFFER, 4, NULL, GL_STATIC_DRAW); // allocate 152 bytes of memory
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboStopWatch);
     
    glUniformBlockBinding(frameShader->ID, glGetUniformBlockIndex(frameShader->ID, "StopWatch"), 2); 
    
    glGenBuffers(1, &uboDistanceFog);
    glBindBuffer(GL_UNIFORM_BUFFER, uboDistanceFog);
    glBufferData(GL_UNIFORM_BUFFER, 32, NULL, GL_DYNAMIC_DRAW); // allocate 152 bytes of memory
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 3, uboDistanceFog);
    
    glGenTextures(1, &skyTexture.id); 
    glUniformBlockBinding(skyShader->ID, glGetUniformBlockIndex(skyShader->ID, "DistanceFog"), 3);
}

Renderer::~Renderer() {  
    
}   

void Renderer::setWorld(World *world_) {
    world = world_;
    
   glGenVertexArrays(1, &sVAO);
   glBindVertexArray(sVAO);
   
   glGenBuffers(1, &sVBO);
   glBindBuffer(GL_ARRAY_BUFFER, sVBO);

   glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), (world->getSkyVertices()), GL_STATIC_DRAW);
   
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   glBindVertexArray(0);
}

void Renderer::setCamera(Camera *camera_) {
    camera = camera_;
    updateViewProj();
    updateCamPos();
}

void Renderer::updateUniformStopWatch() {
    glBindBuffer(GL_UNIFORM_BUFFER, uboStopWatch);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 4, (void*) &timeT);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::updateViewProj() {
    glBindBuffer(GL_UNIFORM_BUFFER, uboViewProj);
    viewMat = glm::lookAt(camera->posVec,camera->dirVec+camera->posVec, glm::vec3(0.0,1.0,0.0));
    
    glm::mat4 viewProj = projMat * viewMat;
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, glm::value_ptr(viewProj));

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


void Renderer::updateDistanceFog() {
    glBindBuffer(GL_UNIFORM_BUFFER, uboDistanceFog);
    auto w = world->getWeather();
    float fogVars[8];
    fogVars[0] = w.fogDensity;
    fogVars[1] = w.fogGradient;
    fogVars[2] = frustrumNear; 
    fogVars[3] = frustrumFar;
    fogVars[4] = w.fogColor.x;
    fogVars[5] = w.fogColor.y;
    fogVars[6] = w.fogColor.z;
    fogVars[7] = 1.0f;

    glBufferSubData(GL_UNIFORM_BUFFER, 0, 32, &fogVars[0]); 
 
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::updateLights() {
    DirectionalLight dl = world->getWeather().dirLight;
    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
     
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 16, glm::value_ptr(glm::vec4(0.1,0.1,0.1,0)));
     glBufferSubData(GL_UNIFORM_BUFFER, 16, 16, glm::value_ptr(glm::vec4(0.2,0.2,0.2,0)));
     glBufferSubData(GL_UNIFORM_BUFFER,  2*16,16, glm::value_ptr(glm::vec4(0.2,0.2,0.2,0)));
     glBufferSubData(GL_UNIFORM_BUFFER, 3*16, 16, glm::value_ptr(glm::vec4(0.2,0.2,0.2,0)));
     
    glm::vec4 shineDir = glm::vec4(dl.getShineDir(),0);
    glm::vec4 ambient = glm::vec4(dl.getAmbient(),0);
    glm::vec4 diffuse = glm::vec4(dl.getDiffuse(),0);
    glm::vec4 specular = glm::vec4(dl.getSpecular(),0);
    glm::vec4 camPos = glm::vec4(camera->posVec,0);

    glBufferSubData(GL_UNIFORM_BUFFER, 4*16,16,glm::value_ptr((shineDir)));
     glBufferSubData(GL_UNIFORM_BUFFER, 5*16, 16, glm::value_ptr((ambient)));
     glBufferSubData(GL_UNIFORM_BUFFER, 6*16,16, glm::value_ptr((diffuse)));
     glBufferSubData(GL_UNIFORM_BUFFER, 7*16, 16, glm::value_ptr((specular)));
     glBufferSubData(GL_UNIFORM_BUFFER, 8*16, 16, glm::value_ptr((camPos)));
     glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::updateCamPos() { 
    glBindBuffer(GL_UNIFORM_BUFFER, uboViewProj);
    viewMat = glm::lookAt(camera->posVec,camera->dirVec+camera->posVec, glm::vec3(0.0,1.0,0.0));
    
    glm::mat4 viewProj = projMat * viewMat;
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, glm::value_ptr(viewProj));
    glBindBuffer(GL_UNIFORM_BUFFER,0);
    
    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
     glBufferSubData(GL_UNIFORM_BUFFER, 8*16, 16, glm::value_ptr(glm::vec4(camera->posVec,0)));
     glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::loadSkyBoxData() {
    int imageWidth, imageHeight, channels;
    unsigned char* imageData = NULL;
    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture.id);

    std::vector<std::string>*skyFiles = world->getSkyTextureFiles();
    for (unsigned int i = 0; i < skyFiles->size(); i++) {
        imageData = stbi_load(skyFiles->at(i).c_str(), &imageWidth, &imageHeight, &channels, 0);
        if (imageData) {
            GLenum format = 4;
                   if (channels == 1)
                       format = GL_RED;
                   else if (channels == 3) 
                       format = GL_RGB; 
                   else if (channels == 4)
                       format = GL_RGBA;
            glTexImage2D(
                         GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_SRGB, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, imageData
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
    
}

void Renderer::checkForUpdates() { // spaghetti
    Updates updates = world->checkforUpdates();

    if(updates.lightingUpdate == true) {
        world->updateCleared(0);
        updateLights(); 
    }
    if(updates.fogUpdate == true) {
        world->updateCleared(1);
        updateDistanceFog();
    }
    if(updates.skyUpdate == true) {
        world->updateCleared(3);
        loadSkyBoxData(); 
    }
}     

void Renderer::renderInitial() {
    checkForUpdates();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
    glBindFramebuffer(GL_FRAMEBUFFER, frame2C.fbo); //draw to 2C framebuffer
    glViewport(0, 0, frame2C.width, frame2C.height);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    updateCamPos();
    updateUniformStopWatch();
    renderSky();  
}

void Renderer::renderFinal() { 
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(frame2C.fvao);
    bool horizontal = true;
    bool first_iteration = true;
    int amount = 6;
    blurShader->use();  
    glActiveTexture(GL_TEXTURE0);  
    
   // resizeViewPort();
    glViewport(0, 0, frame0.width, frame0.height);
   // glBindFramebuffer(GL_FRAMEBUFFER, frame0.fbo);
    for (unsigned int i = 0; i < amount; i++)
    {
        if (horizontal) {
            glBindFramebuffer(GL_FRAMEBUFFER, frame0.fbo);
        }
        else {
            glBindFramebuffer(GL_FRAMEBUFFER, frame1.fbo);
        }
        blurShader->setBool("horizontal", horizontal);
        if (first_iteration) {
            glBindTexture(GL_TEXTURE_2D, frame2C.ftexture1);
        }
        else if (horizontal) {
            glBindTexture(GL_TEXTURE_2D, frame1.ftexture);
        }  
        else {
            glBindTexture(GL_TEXTURE_2D, frame0.ftexture);
        }
        glDrawArrays(GL_TRIANGLES, 0, 6);
        horizontal = (!horizontal);
        if (first_iteration)
            first_iteration = false;
    }
             
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    resizeViewPort();
    
    glClearColor(0.0f,1.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    frameShader->use();
    glBindVertexArray(frame2C.fvao);
    glDisable(GL_DEPTH_TEST);
    
    glActiveTexture(GL_TEXTURE1);
    glUniform1i(glGetUniformLocation(frameShader->ID, "noise"), 1);
    glBindTexture(GL_TEXTURE_2D, noise.id);
    
    glActiveTexture(GL_TEXTURE2);
    glUniform1i(glGetUniformLocation(frameShader->ID, "fbotexture1"), 2);
   glBindTexture(GL_TEXTURE_2D, frame1.ftexture);
     
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(frameShader->ID, "fbotexture"), 0);
    glBindTexture(GL_TEXTURE_2D, frame2C.ftexture0);
    
    glActiveTexture(GL_TEXTURE4);
    glUniform1i(glGetUniformLocation(frameShader->ID, "gradient"), 4);
    glBindTexture(GL_TEXTURE_2D, gradient.id);
        
    glUniform1i(glGetUniformLocation(frameShader->ID, "blur"), world->blur);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);  
     
    timeT += (float)glfwGetTime();
}


void Renderer::renderUI(uiPiece* uip) {
    Shader* s = uip->getShader();
    s->use();
    uip->bind();
    bindTextures(s, uip->getTextureMap());
    glDrawElements(GL_TRIANGLES, uip->getNumIndices(), GL_UNSIGNED_INT, (void*) 0);
    uip->unbind();
}

void Renderer::renderSky() {
    //sky
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDepthMask(GL_FALSE);
    skyShader->use();
     
    glUniform1f(glGetUniformLocation(skyShader->ID, "brightness"), 1.0);
    viewMat = glm::lookAt(camera->posVec,camera->dirVec+camera->posVec, glm::vec3(0.0,1.0,0.0));
    glm::mat4 camViewMat = glm::mat4(glm::mat3(viewMat));
    camViewMat = projMat * camViewMat;
    glUniformMatrix4fv(glGetUniformLocation(skyShader->ID, "viewProjMat2"), 1, GL_FALSE, glm::value_ptr(camViewMat));
    
    glBindVertexArray(sVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sVBO);
    
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(skyShader->ID, "skyBox"), 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture.id);
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glDepthMask(GL_TRUE); 
    glDisable(GL_CULL_FACE);
}

 
void Renderer::bindTextures(Shader* shader, Material& map) {
    if (map.diffuse.id != -1) {
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->ID, "diffuse"), 0);
    glBindTexture(map.diffuse.textureTarget, map.diffuse.id);
    }
    
    if (map.specular.id != -1) {
        glActiveTexture(GL_TEXTURE1);
        glUniform1i(glGetUniformLocation(shader->ID, "specular"), 1);
        glBindTexture(GL_TEXTURE_2D, map.specular.id);
    }
     
    if (map.normMap.id != -1) {
    glActiveTexture(GL_TEXTURE2);
      glUniform1i(glGetUniformLocation(shader->ID, "normMap"), 2);
      glBindTexture(GL_TEXTURE_2D, map.normMap.id);
    } 
    
    if (glGetUniformLocation(shader->ID, "voronoi") != -1) {
    glActiveTexture(GL_TEXTURE3);
      glUniform1i(glGetUniformLocation(shader->ID, "voronoi"), 3);
      glBindTexture(GL_TEXTURE_2D, voronoi.id);
    }
     
    if (glGetUniformLocation(shader->ID, "noise") != -1) {
        glActiveTexture(GL_TEXTURE6);
        glUniform1i(glGetUniformLocation(shader->ID, "noise"), 6);
        glBindTexture(GL_TEXTURE_2D, gradient.id);
    }
    
    if (glGetUniformLocation(shader->ID, "gradient") != -1) {
        glActiveTexture(GL_TEXTURE5);
        glUniform1i(glGetUniformLocation(shader->ID, "gradient"), 5);
        glBindTexture(GL_TEXTURE_2D, gradient.id);
    }
    glActiveTexture(GL_TEXTURE0);
}


void Renderer::renderActor(GraphicsObject* r) {
    r->bind();  
    Shader* s = r->getShader();  
    s->use();  
    Material& map = r->getTextureMap(); 
    bindTextures(s, map);
    glDrawElements(GL_TRIANGLES, r->getNumIndices(), GL_UNSIGNED_INT, (void*) 0);
    r->unbind();
} 
 

void Renderer::renderParticles(GraphicsObject* r, int instanceCount) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(GL_FALSE);
     
    r->bind();
    Shader* s = r->getShader();
    s->use(); 
    Material& map = r->getTextureMap();
    bindTextures(s, map);
    
    glDrawElementsInstanced(r->getDrawTarget(), r->getNumIndices(), GL_UNSIGNED_INT, (void*) 0, instanceCount);
    r->unbind();
    glDepthMask(GL_TRUE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::renderText(uiText* text) {
    text->getShader()->use();
    text->bind();
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(text->getShader()->ID, "textTexture"), 0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, text->getTextureMap().diffuse.id); //hahahaha
    glDrawElements(GL_TRIANGLES, text->getNumIndices(), GL_UNSIGNED_INT, (void*) 0);
    text->unbind();
}
   
void Renderer::renderTerrain(GraphicsObject* r) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    r->bind();
    Shader* s = r->getShader(); 
    s->use();
    Material& map = r->getTextureMap();
    bindTextures(s, map);
    glDrawElements(GL_TRIANGLES, r->getNumIndices(), GL_UNSIGNED_INT, (void*) 0);
    r->unbind();
    glDisable(GL_CULL_FACE);
}

void Renderer::renderFoliage(GraphicsObject* r) {
    glDepthMask(GL_FALSE);
    r->bind();
    Shader* s = r->getShader();
    s->use();
    Material& map = r->getTextureMap();
    bindTextures(s, map);
    glDrawElements(GL_TRIANGLES, r->getNumIndices(), GL_UNSIGNED_INT, (void*) 0);
    r->unbind();
    glDepthMask(GL_TRUE);
}
 
void Renderer::resizeViewPort() {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}
 
void Renderer::updateAllUniblocks() {
    updateLights();
    updateCamPos();
    updateDistanceFog();
    updateViewProj();
}
 
 
 
