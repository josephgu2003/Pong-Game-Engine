//
//  Renderer.cpp
//  Pong
//
//  Created by Joseph Gu on 5/3/21.
//
#define WINDOW_WIDTH 2000.0
#define WINDOW_HEIGHT 1300.0

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
#include "Mesh.hpp"
#include "Camera.hpp"

GLuint uboViewProj;
 GLuint uboLights;
 GLuint uboStopWatch;

void Renderer::bindShaderUniblock(Shader* shader, Uniblock block) {
    if (block == ViewProj) {
    glBindBuffer(GL_UNIFORM_BUFFER, uboViewProj);
    glUniformBlockBinding(shader->ID, glGetUniformBlockIndex(shader->ID, "ViewProj"), 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboViewProj);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
 
    if (block == Lights) {
    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
    glUniformBlockBinding(shader->ID, glGetUniformBlockIndex(shader->ID, "Lights"), 1);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboLights);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
    
    if (block == StopWatch) {
    glBindBuffer(GL_UNIFORM_BUFFER, uboStopWatch);
    glUniformBlockBinding(shader->ID, glGetUniformBlockIndex(shader->ID, "StopWatch"), 2);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboStopWatch);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}

Renderer::Renderer() {
    skyShader = new Shader("Shaders/SkyVertexShader.vs", "Shaders/SkyFragmentShader.fs");

    textShader = new Shader("Shaders/Simple2D.vs", "Shaders/TextFShader.fs");
    sketchShader = new Shader("Shaders/SketchVShader.vs", "Shaders/SketchFShader.fs");
    blurShader = new Shader("Shaders/Simple2D.vs","Shaders/BloomFShader.fs");
    frameShader = new Shader("Shaders/Simple2D.vs", "Shaders/FBufferFShader.fs");

    viewMat = glm::mat4(1);
    float ratio = (WINDOW_WIDTH)/(WINDOW_HEIGHT);
    projMat = glm::perspective(glm::radians(60.0f), ratio, 0.005f, 400.0f);
    timeT = 0;
    AssetManager::loadTexture(TEX_GRADIENT, &gradient, false);
    AssetManager::loadTexture("Resources/Utility/noise.png", &noise, false);
 
    AssetManager::loadTexture(TEX_EMPTY, &texture, false);
      
    AssetManager::generateFramebuffer2Color(&frame2C, WINDOW_WIDTH, WINDOW_HEIGHT);
    AssetManager::generateFramebuffer(&frame0, GL_RGBA16F, 0.5* WINDOW_WIDTH , 0.5 * WINDOW_HEIGHT);
    AssetManager::generateFramebuffer(&frame1, GL_RGBA16F,  0.5* WINDOW_WIDTH , 0.5 * WINDOW_HEIGHT);

    std::vector<int> vector {3,1}; 
    pointParticles.init(1000, 500000, vector,  VERTEX_SIMPLEVERTEX, GL_POINTS, GL_DYNAMIC_DRAW);
    std::vector<int> vector2 {16,1};
    quadParticles.init(1000, 500000, vector2, VERTEX_SIMPLEVERTEX, GL_TRIANGLES, GL_DYNAMIC_DRAW);
    soundTexts.init(10000, 10000, VERTEX_SIMPLEVERTEX, GL_TRIANGLES, GL_DYNAMIC_DRAW);
}

Renderer::~Renderer() { 
    
} 

void Renderer::setWorld(World *world_) {
    world = world_;
}

void Renderer::setCamera(Camera *camera_) {
    
    camera = camera_;

    glEnable(GL_POINT_SPRITE);
    glEnable(GL_PROGRAM_POINT_SIZE);
    
    glGenBuffers(1, &uboViewProj);
    glBindBuffer(GL_UNIFORM_BUFFER, uboViewProj);
    glBufferData(GL_UNIFORM_BUFFER, 64, NULL, GL_DYNAMIC_DRAW); // allocate 152 bytes of memory
    
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glUniformBlockBinding(sketchShader->ID, glGetUniformBlockIndex(sketchShader->ID, "ViewProj"), 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboViewProj);

    glGenBuffers(1, &uboLights);
    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
    glBufferData(GL_UNIFORM_BUFFER, 9*sizeof(glm::vec4), NULL, GL_STATIC_DRAW); // allocate 152 bytes of memory
    
    
    glUniformBlockBinding(sketchShader->ID, glGetUniformBlockIndex(sketchShader->ID, "Lights"), 1);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboLights);
    
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    updateUniformBlocks();
    
    glGenBuffers(1, &uboStopWatch);
    glBindBuffer(GL_UNIFORM_BUFFER, uboStopWatch);
    glBufferData(GL_UNIFORM_BUFFER, 4, NULL, GL_STATIC_DRAW); // allocate 152 bytes of memory
    glUniformBlockBinding(frameShader->ID, glGetUniformBlockIndex(frameShader->ID, "StopWatch"), 2);
    glUniformBlockBinding(frameShader->ID, glGetUniformBlockIndex(frameShader->ID, "StopWatch"), 2);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboStopWatch);
    
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    updateUniformBlocks();
}

void Renderer::updateUniformBlocks() {
//

    updateViewProj();
    updateLights();
    updateCamPos();
    updateUniformStopWatch();
    
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

void Renderer::loadActorData() {
    std::vector<TBNBWVertex> actorVertices;
    std::vector<GLuint> actorIndices;
    
    for (int i = 0; i<world->getActorsCount(); i++) { // iterate over actors
        std::vector<Mesh>* meshes = world->getNthActor(i)->getComponent<GraphicsComponent>()->getModel()->getMeshes(); 
    
        for (int j = 0; j<meshes->size(); j++) { // iterate over meshes
            int marker = actorIndices.size();
           const std::vector<std::unique_ptr<AnyVertex>>& vertices = meshes->at(j).getVertices();
        
            for (int c = 0; c < vertices.size(); c++) {
                actorVertices.push_back((*static_cast<TBNBWVertex*>(vertices.at(c).get()))); // crash here bad access  
            }
              
            actorIndices.insert(actorIndices.end(), meshes->at(j).getIndices().begin(), meshes->at(j).getIndices().end());
            for ( marker; marker != actorIndices.size(); marker++) {
            actorIndices[marker] += actorVertices.size() - meshes->at(j).getVertices().size();
            }
        }
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, actorVertices.size() * sizeof(TBNBWVertex), actorVertices.data(), GL_STATIC_DRAW);
     
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, actorIndices.size() * sizeof(GLuint), actorIndices.data(), GL_STATIC_DRAW);
    
    VertexLoader::setupVAOAttribs(VERTEX_TBNBWVERTEX);
    
    glBindVertexArray(0); 
}

void Renderer::loadMapData() {
    std::vector<TBNVertex> mapVertices;
    
    GraphicsComponent& gc = world->getMap().getGraphics();
    VertexData* mesh = gc.getVertexData();

    const std::vector<std::unique_ptr<AnyVertex>>& vertices = mesh->getVertices();
    
    mapVertices.resize(vertices.size());
    
    for (int c = 0; c < vertices.size(); c++) {
                mapVertices[c] = *static_cast<TBNVertex*>(vertices.at(c).get());
    }
     
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
      
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER,  sizeof(TBNVertex)*vertices.size(), mapVertices.data(), GL_STATIC_DRAW);
      
    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,  mesh->getIndices().size() * sizeof(GLuint), &mesh->getIndices().at(0), GL_STATIC_DRAW);
       
    VertexLoader::setupVAOAttribs(VERTEX_TBNVERTEX);
      
    glBindVertexArray(0);
}

void Renderer::loadSkyBoxData() {
    int imageWidth, imageHeight, channels;
    unsigned char* imageData = NULL;
    glGenTextures(1, &skyTexture.id);
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
    
    std::vector<ParticleEffect*> particleData = world->getParticleEffects();
    std::vector<ParticleEffect*> deletedParticles = loadedParticles;
    // subtract first or add first?
    for (int i = 0; i < particleData.size(); i++) { // loop world particles
        bool updateNeeded = true;
        for (int j = 0; j < loadedParticles.size(); j++) { // compare to known particles
            if (particleData.at(i) == loadedParticles.at(j)) {
                deletedParticles.erase(std::find(deletedParticles.begin(), deletedParticles.end(), particleData.at(i)));
                updateNeeded = false; // if match, no update needed
                break;
            }
        }
        
        if (updateNeeded) {
            loadedParticles.push_back(particleData.at(i));
            if (particleData.at(i)->drawTarget == GL_TRIANGLES) {
                quadParticles.updateVertexData(particleData.at(i)->graphics.getVertexData(), particleData.at(i));
            }
            if (particleData.at(i)->drawTarget == GL_POINTS) {
                pointParticles.updateVertexData(particleData.at(i)->graphics.getVertexData(), particleData.at(i));
            }
        }
       if (particleData.at(i)->drawTarget == GL_TRIANGLES) {
            std::vector<float> instanceData;
           int numParticles = particleData.at(i)->getNumParticles();
           int vecLength = 17*numParticles;
           instanceData.resize(vecLength);
           int counter = 0;
            for (int c = 0; c < particleData.at(i)->getNumParticles(); c++) {
                glm::vec3& pos = particleData.at(i)->getNthParticle(c).posVec;
                glm::mat4 modelMatr = glm::mat4(1.0f);
                 
                modelMatr = glm::translate(modelMatr, pos);
                modelMatr = glm::rotate(modelMatr,    -glm::radians(particleData.at(i)->getNthParticle(c).pyrAngles.y), glm::vec3(0,1,0));

                float* mat = glm::value_ptr(modelMatr[0]);
                for (int l = 0; l < 16; l++) {
                    instanceData[counter] = (mat[l]);
                    counter++;
                }
                instanceData[counter] = 10.0;
                counter++; 
            }
            quadParticles.updateInstanceData(particleData.at(i)->graphics.getVertexData(),instanceData, particleData.at(i));
        }
        
        if (particleData.at(i)->drawTarget == GL_POINTS) {
            std::vector<float> instanceData;
            int numParticles = particleData.at(i)->getNumParticles();
            int vecLength = 4*numParticles;
            instanceData.resize(vecLength);
            int counter = 0;
            for (int c = 0; c < particleData.at(i)->getNumParticles(); c++) {
                glm::vec3& pos = particleData.at(i)->getNthParticle(c).posVec;
                instanceData[counter] = pos.x;
                counter++;
                instanceData[counter] = pos.y;
                counter++;
                instanceData[counter] = pos.z;
                counter++;
                instanceData[counter] = (particleData.at(i)->getNthParticle(c).duration);
                counter++;
            }
            pointParticles.updateInstanceData(particleData.at(i)->graphics.getVertexData(),instanceData, particleData.at(i));
        }
    }
    
    for (int i = 0; i < deletedParticles.size(); i++) { // delete particles
        int bytes = 0;
        for (int c = 0; c < deletedParticles.at(i)->getNumParticles(); c++) {
            bytes += 4 * sizeof(float);
        }
        if (deletedParticles.at(i)->drawTarget == GL_TRIANGLES) {
            quadParticles.deleteInstanceData(bytes, deletedParticles.at(i));
            quadParticles.deleteVertexData(deletedParticles.at(i));

            loadedParticles.erase(std::find(loadedParticles.begin(), loadedParticles.end(), deletedParticles.at(i)));
        }
        if (deletedParticles.at(i)->drawTarget == GL_POINTS) {
            pointParticles.deleteInstanceData(bytes, deletedParticles.at(i));

            pointParticles.deleteVertexData(deletedParticles.at(i));
           
            loadedParticles.erase(std::find(loadedParticles.begin(), loadedParticles.end(), deletedParticles.at(i)));
        }
    } 
}

void Renderer::loadQuadData() {
    std::vector<Vertex> quadVertices;
    std::vector<GLuint> quadIndices;

    for (int i = 0; i<world->getQuads()->size(); i++) { // iterate over actors
        quadVertices.insert(quadVertices.end(), world->getQuads()->at(i)->vertices.begin(), world->getQuads()->at(i)->vertices.end());
        quadIndices.insert(quadIndices.end(), world->getQuads()->at(i)->indices.begin(), world->getQuads()->at(i)->indices.end());
    }

    glGenVertexArrays(1, &qVAO);
    glBindVertexArray(qVAO);
    
    glGenBuffers(1, &qVBO); 
    glBindBuffer(GL_ARRAY_BUFFER, qVBO);

    glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * sizeof(Vertex), quadVertices.data(), GL_STATIC_DRAW);
    
    glGenBuffers(1, &qEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, qEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, quadIndices.size() * sizeof(GLuint), quadIndices.data(), GL_STATIC_DRAW);
    VertexLoader::setupVAOAttribs(VERTEX_VERTEX);
    
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
      //  world->updateCleared(TEXT_UPDATE);
     //   print(world->getActiveText());
        //wishlist: function to get renderer all the relevant soundtext objects
      //  world->fillBatchTextData(&soundTexts, glm::vec3(camera->getPos()));
    }
    if(updates.lightingUpdate == true) {
        world->updateCleared(LIGHTING_UPDATE);
        updateLights(); 
    }
}

void Renderer::render() {
    checkForUpdates();
    loadParticleData();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
    glBindFramebuffer(GL_FRAMEBUFFER, frame2C.fbo); //draw to 2C framebuffer
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    updateCamPos();
    updateUniformStopWatch();
   renderSky();
    renderActors();
    renderMap();
    renderParticles();
    renderQuads();
    world->drawText(this);
}
void Renderer::render2() {
     
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(frame2C.fvao);
    bool horizontal = true;
    bool first_iteration = true;
    int amount = 6;
    blurShader->use();  
    glActiveTexture(GL_TEXTURE0);
    glViewport(0,0, 1000, 650);
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
    
    glViewport(0,0, 2000, 1300);
             
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
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
      
    glUniform1i(glGetUniformLocation(frameShader->ID, "blur"), world->blur);
    
    glDrawArrays(GL_TRIANGLES, 0, 6); 
     
    timeT += (float)glfwGetTime();
}



void Renderer::renderUI(uiPiece* uip) {
    if (!ui.lock()) return;
    Shader* s = uip->getShader();
    s->use();
    uip->bind();
    bindTextures(s, uip->getTextureMap());
    glDrawElements(GL_TRIANGLES, uip->getNumIndices(), GL_UNSIGNED_INT, (void*) 0);
    uip->unbind();
}



void Renderer::renderSky() {
    //sky
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDepthMask(GL_FALSE);
    skyShader->use();
    glUniform1f(glGetUniformLocation(skyShader->ID, "brightness"), 2.5*world->getWeather().dirLight.getDiffuse().x);
    viewMat = glm::lookAt(camera->posVec,camera->dirVec+camera->posVec, glm::vec3(0.0,1.0,0.0));
    glm::mat4 camViewMat = glm::mat4(glm::mat3(viewMat));
    camViewMat = projMat * camViewMat;
    glUniformMatrix4fv(glGetUniformLocation(skyShader->ID, "viewProjMat2"), 1, GL_FALSE, glm::value_ptr(camViewMat));
    glBindVertexArray(sVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sVBO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glDepthMask(GL_TRUE);
    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);
}

void Renderer::renderQuads() {
    glEnable(GL_BLEND);
    if (world->getQuads()->size() > 0) {
        int indiceCount = 0;
        for (int i = 0; i < world->getQuads()->size(); i++) {
    Quad* quad = world->getQuads()->at(i);
    sketchShader->use();
    glBindVertexArray(qVAO);
    glBindBuffer(GL_ARRAY_BUFFER, qVBO);
    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(Vertex)*quad->vertices.size(),quad->vertices.data());
    glm::mat4 modelMat = glm::mat4(1.0f);

        glm::vec3 rotations = glm::vec3(glm::radians(quad->rotations.x),glm::radians(quad->rotations.y),glm::radians(quad->rotations.z));

    glUniformMatrix4fv(glGetUniformLocation(sketchShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(sketchShader->ID, "diffuse"), 0);
            glUniform1f(glGetUniformLocation(sketchShader->ID, "alpha"), quad->alpha);
        glBindTexture(GL_TEXTURE_2D, world->getQuads()->at(i)->texture.id);
        glDrawElements(GL_TRIANGLES, quad->indices.size(), GL_UNSIGNED_INT, (void*) indiceCount);
            
            indiceCount += quad->indices.size()*sizeof(GLuint);
    }
    }
    glDepthMask(GL_TRUE);
}

void Renderer::renderMap() {
    GraphicsComponent& gc = world->getMap().getGraphics();
    Shader* shader = gc.getShader();
    shader->use();
    
    glBindVertexArray(mVAO);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture.id);
    
    TextureMaps& map = gc.getVertexData()->getTextures();
    
    bindTextures(shader, map);
    
    glDrawElements(GL_TRIANGLES, gc.getVertexData()->getIndices().size(), GL_UNSIGNED_INT, 0);
    
    glActiveTexture(GL_TEXTURE0);

}

void Renderer::renderActors() {
    // actors
    glBindVertexArray(VAO);
    int indiceCount = 0;
    for (int i = 0; i<world->getActorsCount(); i++) {
        GraphicsComponent* comp = world->getNthActor(i)->getComponent<GraphicsComponent>();
    std::vector<Mesh>* meshes = comp->getModel()->getMeshes();
        Shader* shader = comp->getShader();
        shader->use();
        for (int j = 0; j < meshes->size(); j++) {
            TextureMaps& map = meshes->at(j).getTextures();
  
            bindTextures(shader, map);

            glDrawElements(GL_TRIANGLES, meshes->at(j).getIndices().size(), GL_UNSIGNED_INT, (void*) indiceCount);
            
            indiceCount += (meshes->at(j).getIndices().size())*sizeof(GLuint);
        
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}

void Renderer::renderParticles() {
    //particles
   glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
   glDepthMask(GL_FALSE);
    for (int i = 0; i < world->getParticleEffects().size(); i++) {
        ParticleEffect& effect = *world->getParticleEffects().at(i);
        Batch<ParticleEffect>* batch;
        int elementsPerInstance = 3;
        if (effect.drawTarget == GL_TRIANGLES) {
            batch = &quadParticles;
            elementsPerInstance = 6;
        }
        if (effect.drawTarget == GL_POINTS) {
            batch = &pointParticles;
    
            elementsPerInstance = 1;
        }
        batch->bindVAO();
        batch->bindInstanceBuffer(&effect);
        Shader* shaderRef = effect.graphics.getShader();
        shaderRef->use();
        GLuint shader = shaderRef->ID;
        
        glActiveTexture(GL_TEXTURE1);
        if (glGetUniformLocation(shader, "gradient") != -1) {
        glUniform1i(glGetUniformLocation(shader, "gradient"), 1);
        glBindTexture(GL_TEXTURE_2D, gradient.id);
        }
        if (glGetUniformLocation(shader, "diffuse") != -1) {
         glActiveTexture(GL_TEXTURE0); 
            glUniform1i(glGetUniformLocation(shader, "diffuse"), 0);
        glBindTexture(effect.textureTarget, effect.texture.id);
        }
        
        glDrawElementsInstanced(effect.drawTarget, elementsPerInstance, GL_UNSIGNED_INT, (void*)(batch->getIndexByteStride(&effect)), effect.getNumParticles());
        glBindTexture(effect.textureTarget, 0);
    } 
    glBindVertexArray(0); 
    glActiveTexture(GL_TEXTURE0);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glDepthMask(GL_TRUE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

  
void Renderer::renderText(uiText* text) {
   /** textShader->use();
    glEnable(GL_DEPTH_TEST); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //  glBindVertexArray(tVAO);
    soundTexts.bindVAO();   
    int verticeCount = 0;   
    for (int i = 0; i < soundTexts.getNumDrawables(); i++) { // wtf bro u really texture swapping per character??
        glBindTexture(GL_TEXTURE_2D_ARRAY,   soundTexts.getVertexDataMap().at(i).first->data.getTextures().diffuse.id); //hahahaha
        glDrawElements(GL_TRIANGLES, soundTexts.getVertexDataMap().at(i).first->data.getIndices().size(), GL_UNSIGNED_INT, (void*) verticeCount);
        verticeCount += soundTexts.getVertexDataMap().at(i).first->data.getIndices().size();
    }
    soundTexts.unbindVAO(); **/
    text->getShader()->use(); 
  //  glBindVertexArray(tVAO);
    text->bind();
    glBindTexture(GL_TEXTURE_2D_ARRAY, text->getTextureMap().diffuse.id); //hahahaha
    glDrawElements(GL_TRIANGLES, text->getNumIndices(), GL_UNSIGNED_INT, (void*) 0);
    text->unbind(); 
} 
   
 
void Renderer::bindTextures(Shader* shader, TextureMaps& map) {
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
    
    if (map.voronoi.id != -1) { 
    glActiveTexture(GL_TEXTURE3);
      glUniform1i(glGetUniformLocation(shader->ID, "voronoi"), 3);
      glBindTexture(GL_TEXTURE_2D, map.voronoi.id);
    }
    
    if (map.voronoi.id != -1) {
    glActiveTexture(GL_TEXTURE4);
      glUniform1i(glGetUniformLocation(shader->ID, "noise"), 4);
      glBindTexture(GL_TEXTURE_2D, noise.id);
    }
    
    glActiveTexture(GL_TEXTURE0);
}

void Renderer::setUI(const std::shared_ptr<uiLayout>& ui_) {
    ui = ui_;
}
