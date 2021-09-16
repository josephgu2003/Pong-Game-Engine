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

GLuint uboViewProj;
 GLuint uboLights;
 GLuint uboStopWatch;

Renderer::Renderer() { 
    exposure = 0;
    skyShader = new Shader("Shaders/SkyVertexShader.vs", "Shaders/SkyFragmentShader.fs");

    textShader = new Shader("Shaders/Simple2D.vs", "Shaders/TextFShader.fs");
    sketchShader = new Shader("Shaders/SketchVShader.vs", "Shaders/SketchFShader.fs");
    blurShader = new Shader("Shaders/Simple2D.vs","Shaders/BloomFShader.fs");
    frameShader = new Shader("Shaders/Simple2D.vs", "Shaders/FBufferFShader.fs");

    modelMat = glm::mat4(1);
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
            std::vector<std::shared_ptr<AnyVertex>>& vertices = meshes->at(j).getVertices();
            
            for (int c = 0; c < vertices.size(); c++) {
                actorVertices.push_back(*static_cast<TBNBWVertex*>(vertices.at(c).get()));
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
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TBNBWVertex), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TBNBWVertex), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TBNBWVertex), (void*)(2*sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);
    
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TBNBWVertex), (void*)(2*sizeof(glm::vec3)+sizeof(glm::vec2)));
    glEnableVertexAttribArray(3);
    
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(TBNBWVertex), (void*)(3*sizeof(glm::vec3)+sizeof(glm::vec2)));
    glEnableVertexAttribArray(4);
    
    glVertexAttribIPointer(5, MAX_BONE_WEIGHTS, GL_INT, sizeof(TBNBWVertex), (void*)(4*sizeof(glm::vec3)+sizeof(glm::vec2)));
    glEnableVertexAttribArray(5);
    
    glVertexAttribPointer(6, MAX_BONE_WEIGHTS, GL_FLOAT, GL_FALSE, sizeof(TBNBWVertex), (void*)(MAX_BONE_WEIGHTS*sizeof(int)+4*sizeof(glm::vec3)+sizeof(glm::vec2)));
    glEnableVertexAttribArray(6);
    
    glBindVertexArray(0); 
}

void Renderer::loadMapData() {
    std::vector<TBNVertex> mapVertices;
    
    GraphicsComponent& gc = world->getMap().getGraphics();
    VertexData* mesh = gc.getVertexData();

            std::vector<std::shared_ptr<AnyVertex>>& vertices = mesh->getVertices();
    
    mapVertices.resize(vertices.size());
    
    int counter = 0;
    
            for (int c = 0; c < vertices.size(); c++) {
                mapVertices[counter] = *static_cast<TBNVertex*>(vertices.at(c).get());
                counter++;
            }

    glGenVertexArrays(1, &mVAO);
      glBindVertexArray(mVAO);
      
      glGenBuffers(1, &mVBO);
      glBindBuffer(GL_ARRAY_BUFFER, mVBO);

      glBufferData(GL_ARRAY_BUFFER,  sizeof(TBNVertex)*mesh->getVertices().size(), mapVertices.data(), GL_STATIC_DRAW);
      
      glGenBuffers(1, &mEBO);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,  mesh->getIndices().size() * sizeof(GLuint), &mesh->getIndices().at(0), GL_STATIC_DRAW);
       
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(1); 
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)(2*sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);
    
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)(2*sizeof(glm::vec3)+sizeof(glm::vec2)));
    glEnableVertexAttribArray(3);
    
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)(3*sizeof(glm::vec3)+sizeof(glm::vec2)));
    glEnableVertexAttribArray(4);
      
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
                quadParticles.updateVertexData(particleData.at(i)->graphics.getVertexData());
            }
            if (particleData.at(i)->drawTarget == GL_POINTS) {
                pointParticles.updateVertexData(particleData.at(i)->graphics.getVertexData());
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
               // instanceData[counter] = (particleData.at(i)->getNthParticle(c).duration);
                instanceData[counter] = 10.0;
                counter++; 
            }
            quadParticles.updateInstanceData(particleData.at(i)->graphics.getVertexData(),instanceData);
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
            pointParticles.updateInstanceData(particleData.at(i)->graphics.getVertexData(),instanceData);
        }
    }
    
    for (int i = 0; i < deletedParticles.size(); i++) {
        int bytes = 0;
        for (int c = 0; c < deletedParticles.at(i)->getNumParticles(); c++) {
            bytes += 4 * sizeof(float);
        }
        if (deletedParticles.at(i)->drawTarget == GL_TRIANGLES) {
            quadParticles.deleteInstanceData(deletedParticles.at(i)->graphics.getVertexData(), bytes);
            quadParticles.deleteVertexData(deletedParticles.at(i)->graphics.getVertexData());

            loadedParticles.erase(std::find(loadedParticles.begin(), loadedParticles.end(), deletedParticles.at(i)));
            delete deletedParticles.at(i);
        }
        if (deletedParticles.at(i)->drawTarget == GL_POINTS) {
            pointParticles.deleteInstanceData(deletedParticles.at(i)->graphics.getVertexData(), bytes);

            pointParticles.deleteVertexData(deletedParticles.at(i)->graphics.getVertexData());
           

            loadedParticles.erase(std::find(loadedParticles.begin(), loadedParticles.end(), deletedParticles.at(i)));
            delete deletedParticles.at(i);
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
    renderUI();
    if (screenText.duration > 0.0f) {
        renderText();

    }
    
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
    screenText.duration -= (float)glfwGetTime();
}



void Renderer::renderUI() {
    uiStuff.bindVAO();
    uiShader->use();
    
}


void Renderer::incExposure(float delta) {
    exposure += delta;
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
    modelMat = glm::mat4(1.0f);

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
    modelMat = glm::mat4(1);
    glDepthMask(GL_TRUE);
}

void Renderer::renderMap() {
    
    GraphicsComponent& gc = world->getMap().getGraphics();
    Shader* shader = gc.getShader();
    shader->use();
    
    glBindVertexArray(mVAO);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture.id);
    
    TextureMaps& map = gc.getVertexData()->getTextures();
    
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
            
            if (map.diffuse.id != -1) {
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(glGetUniformLocation(shader->ID, "diffuse"), 0);
            glBindTexture(GL_TEXTURE_2D, map.diffuse.id);
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
            
            glActiveTexture(GL_TEXTURE0);

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
        Batch* batch;
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
        batch->bindInstanceBuffer(effect.graphics.getVertexData());
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
        glDrawElementsInstanced(effect.drawTarget, elementsPerInstance, GL_UNSIGNED_INT, (void*)(batch->getIndexByteStride(effect.graphics.getVertexData())), effect.getNumParticles());
        glBindTexture(effect.textureTarget, 0);
    } 
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDepthMask(GL_TRUE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::print(std::string string) {

    screenText.duration = 3.7; 
    screenText.text = string;
    if (screenText.Characters.size() == 0){
    AssetManager::loadGlyphs("Resources/Glyphs/times.ttf", screenText.Characters);
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
    glEnable(GL_DEPTH_TEST);
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
  *    for (int j = 0; j < world->getParticleEffects()->at(i)->getNumParticles(); j++) {
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
