//
//  Renderer.cpp
//  Pong
//
//  Created by Joseph Gu on 5/3/21.
//

#include "Renderer.hpp"
#include "stb_image.h"
#include <iostream>
#include <map>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "DirectionalLight.hpp"

GLuint uboViewProj;
GLuint uboLights;
GLuint uboStopWatch;

Renderer::Renderer() {

    skyShader = new Shader("Shaders/SkyVertexShader.vs", "Shaders/SkyFragmentShader.fs");

    textShader = new Shader("Shaders/TextVShader.vs", "Shaders/TextFShader.fs");
    sketchShader = new Shader("Shaders/SketchVShader.vs", "Shaders/SketchFShader.fs");
    blurShader = new Shader("Shaders/BloomVShader.vs","Shaders/BloomFShader.fs");
    frameShader = new Shader("Shaders/FBufferVShader.vs", "Shaders/FBufferFShader.fs");

    modelMat = glm::mat4(1);
    viewMat = glm::mat4(1);
    projMat = glm::perspective(glm::radians(50.0f), 1000.0f/800.0f, 0.01f, 100.0f);
    projMat2 = glm::perspective(glm::radians(50.0f), 1000.0f/800.0f, 0.01f, 100.0f);
    timeT = 0;
    gradient = loadTexture(TEX_GRADIENT);
    noise = loadTexture("Resources/Utility/noise.png");

    texture = loadTexture(TEX_EMPTY);
    
    generateFramebuffer2Color(&frame2C, 2000, 1600);
    generateFramebuffer(&frame0, 2000, 1600);
    generateFramebuffer(&frame1, 2000, 1600);

    std::vector<int> vector {3,1};
    pointParticles.init(300, 500000, vector, VERTEX_SIMPLEVERTEX, GL_POINTS, GL_DYNAMIC_DRAW);
    std::vector<int> vector2 {1,16};
    quadParticles.init(300, 500000, vector2, VERTEX_SIMPLEVERTEX, GL_TRIANGLES, GL_DYNAMIC_DRAW);
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
    glBufferData(GL_UNIFORM_BUFFER, 128, NULL, GL_STATIC_DRAW); // allocate 152 bytes of memory
    
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
    glUniformBlockBinding(frame2C.shader.ID, glGetUniformBlockIndex(frame2C.shader.ID, "StopWatch"), 2);
    glUniformBlockBinding(frame2C.shader.ID, glGetUniformBlockIndex(frame2C.shader.ID, "StopWatch"), 2);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboStopWatch);
    
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    updateUniformBlocks();
}

void Renderer::updateUniformBlocks() {
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
    
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, glm::value_ptr(viewMat));
    glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, glm::value_ptr(projMat));
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
    
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, glm::value_ptr(viewMat));
    glBindBuffer(GL_UNIFORM_BUFFER,0);
    
    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
     glBufferSubData(GL_UNIFORM_BUFFER, 8*16, 16, glm::value_ptr(glm::vec4(camera->posVec,0)));
     glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::loadActorData() {
    std::vector<TBNVertex> actorVertices;
    std::vector<GLuint> actorIndices;
    for (int i = 0; i<world->getActorsCount(); i++) { // iterate over actors
        std::vector<Mesh>* meshes = world->getNthActor(i)->model->getMeshes();
        for (int j = 0; j<meshes->size(); j++) { // iterate over meshes
            int marker = actorIndices.size();
            std::vector<std::shared_ptr<AnyVertex>>& vertices = meshes->at(j).getVertices();
            for (int c = 0; c < vertices.size(); c++) {
                actorVertices.push_back(*static_cast<TBNVertex*>(vertices.at(c).get()));
            }
            actorIndices.insert(actorIndices.end(), meshes->at(j).getIndices().begin(), meshes->at(j).getIndices().end());
            for (marker; marker != actorIndices.size(); marker++) {
            actorIndices[marker] += actorVertices.size() - meshes->at(j).getVertices().size();
            }
        }
    }
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, actorVertices.size() * sizeof(TBNVertex), actorVertices.data(), GL_STATIC_DRAW);
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, actorIndices.size() * sizeof(GLuint), actorIndices.data(), GL_STATIC_DRAW);
    
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

void Renderer::loadMapData() {
    std::vector<TBNVertex> mapVertices;
    
    Mesh mesh = world->getMap().getMesh();

            std::vector<std::shared_ptr<AnyVertex>>& vertices = mesh.getVertices();
            for (int c = 0; c < vertices.size(); c++) {
                mapVertices.push_back(*static_cast<TBNVertex*>(vertices.at(c).get()));
            }

    glGenVertexArrays(1, &mVAO);
      glBindVertexArray(mVAO);
      
      glGenBuffers(1, &mVBO);
      glBindBuffer(GL_ARRAY_BUFFER, mVBO);

      glBufferData(GL_ARRAY_BUFFER,  sizeof(TBNVertex)*world->getMap().getMesh().getVertices().size(), mapVertices.data(), GL_STATIC_DRAW);
      
      glGenBuffers(1, &mEBO);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,  world->getMap().getMesh().getIndices().size() * sizeof(GLuint), &world->getMap().getMesh().getIndices().at(0), GL_STATIC_DRAW);
      
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TBNVertex), (void*)0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(TBNVertex), (void*)(3*sizeof(float)));
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  sizeof(TBNVertex), (void*)(6*sizeof(float)));
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
                         0, format, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, imageData
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
    for (int i = 0; i < particleData.size(); i++) {
        bool updateNeeded = true;
        for (int j = 0; j < loadedParticles.size(); j++) {
            if (particleData.at(i) == loadedParticles.at(j)) {
                updateNeeded = false;
                deletedParticles.erase(std::find(deletedParticles.begin(), deletedParticles.end(), particleData.at(i)));
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
                instanceData[counter] = (particleData.at(i)->getNthParticle(c).duration);
                counter++;
                glm::vec3& pos = particleData.at(i)->getNthParticle(c).posVec;
                glm::mat4 modelMatr = glm::mat4(1.0f);
                modelMatr = glm::translate(modelMatr, pos);
                modelMatr = glm::rotate(modelMatr, -glm::radians(-90+camera->getYaw()), glm::vec3(0,1,0));
                modelMatr = glm::rotate(modelMatr, -glm::radians(camera->getPitch()), glm::vec3(1,0,0));
                float* mat = glm::value_ptr(modelMatr[0]);
                for (int l = 0; l < 16; l++) {
                    instanceData[counter] = (mat[l]);
                    counter++;
                }
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
        std::vector<float> instanceData;
        for (int c = 0; c < deletedParticles.at(i)->getNumParticles(); c++) {
            glm::vec3& pos = deletedParticles.at(i)->getNthParticle(c).posVec;
            instanceData.push_back(pos.x);
            instanceData.push_back(pos.y);
            instanceData.push_back(pos.z);
            instanceData.push_back(deletedParticles.at(i)->getNthParticle(c).duration);
        }
        if (deletedParticles.at(i)->drawTarget == GL_TRIANGLES) {
            quadParticles.deleteVertexData(deletedParticles.at(i)->graphics.getVertexData());
            quadParticles.deleteInstanceData(deletedParticles.at(i)->graphics.getVertexData(), instanceData);
        }
        if (deletedParticles.at(i)->drawTarget == GL_POINTS) {
            pointParticles.deleteVertexData(deletedParticles.at(i)->graphics.getVertexData());
            pointParticles.deleteInstanceData(deletedParticles.at(i)->graphics.getVertexData(), instanceData);
        }
    }
}


void Renderer::updateParticleBatches() {
    
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
}

void Renderer::render() {
    checkForUpdates();
    loadParticleData();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, frame2C.fbo); //draw to 2C framebuffer
    glEnable(GL_DEPTH_TEST);
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateCamPos();
    updateUniformStopWatch();
    renderSky();
    renderMap();
    renderActors();
    renderParticles();
    renderQuads();
    renderUI();
    
    if (screenText.duration > 0) {
        renderText();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(frame2C.fvao);
    bool horizontal = true;
    bool first_iteration = true;
    int amount = 10;
    blurShader->use(); 
    glActiveTexture(GL_TEXTURE0);
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
       horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
     
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    frameShader->use();
    glActiveTexture(GL_TEXTURE1);
    glUniform1i(glGetUniformLocation(frameShader->ID, "noise"), 1);
    glBindTexture(GL_TEXTURE_2D, noise);
    
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(frameShader->ID, "fbotexture1"), 0);
    glBindTexture(GL_TEXTURE_2D, frame1.ftexture);
    
    glActiveTexture(GL_TEXTURE2);
    glUniform1i(glGetUniformLocation(frameShader->ID, "fbotexture"), 2);
    glBindTexture(GL_TEXTURE_2D, frame2C.ftexture0);
    
    glUniform1i(glGetUniformLocation(frameShader->ID, "blur"), world->blur);
    frameShader->setFloat("exposure", 1.0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    timeT += (float)glfwGetTime();
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
    glUniform1f(glGetUniformLocation(skyShader->ID, "brightness"), 0.3);
    viewMat = glm::lookAt(camera->posVec,camera->dirVec+camera->posVec, glm::vec3(0.0,1.0,0.0));
    glm::mat4 camViewMat = glm::mat4(glm::mat3(viewMat));
    glUniformMatrix4fv(glGetUniformLocation(skyShader->ID, "viewMat2"), 1, GL_FALSE, glm::value_ptr(camViewMat));
    glUniformMatrix4fv(glGetUniformLocation(skyShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
    glBindVertexArray(sVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sVBO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
    glEnable(GL_BLEND);
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
  //  modelMat = glm::translate(modelMat, quad->pos);
        glm::vec3 rotations = glm::vec3(glm::radians(quad->rotations.x),glm::radians(quad->rotations.y),glm::radians(quad->rotations.z));
    glm::quat MyQuaternion = glm::quat(rotations);

    glm::mat4 RotationMatrix = toMat4(MyQuaternion);

 //   modelMat = modelMat * RotationMatrix;

    glUniformMatrix4fv(glGetUniformLocation(sketchShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
       // glUniform1f(glGetUniformLocation(sketchShader->ID, "z_offsets"), billowing[0]);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(sketchShader->ID, "texture0"), 0);
            glUniform1f(glGetUniformLocation(sketchShader->ID, "alpha"), quad->alpha);
        glBindTexture(GL_TEXTURE_2D, world->getQuads()->at(0)->texture);
        glDrawElements(GL_TRIANGLES, quad->indices.size(), GL_UNSIGNED_INT, (void*) indiceCount);
            
            indiceCount += quad->indices.size()*sizeof(GLuint);
    }
    }
    modelMat = glm::mat4(1);
    glDepthMask(GL_TRUE);
}


void Renderer::renderMap() {
    //map
   // glActiveTexture(GL_TEXTURE0);
   // glBindTexture(GL_TEXTURE_2D, texture);
    Map& map = world->getMap();
    map.getShader().use();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glBindVertexArray(mVAO);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture);
    
    glActiveTexture(GL_TEXTURE2);
    glUniform1i(glGetUniformLocation(map.getShader().ID, "mapTexture"), 2);
    glBindTexture(GL_TEXTURE_2D, map.getMesh().getTextures().at(0).id);
    
    glActiveTexture(GL_TEXTURE1);
    glUniform1i(glGetUniformLocation(map.getShader().ID, "noise"), 1);
    glBindTexture(GL_TEXTURE_2D, noise);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    glActiveTexture(GL_TEXTURE0);
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
            if (meshes->at(j).getTextures().size() > 0) {
                id1 = meshes->at(j).getTextures().at(0).id;
            } else {
               // id1 = funtex2;
        
            }
            GLuint id2;
            if (meshes->at(j).getTextures().size()>1) {
                id2 = meshes->at(j).getTextures().at(1).id;
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
        int elementsPerInstance;
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
        if (glGetUniformLocation(shader, "texture1") != -1) {
        glUniform1i(glGetUniformLocation(shader, "texture1"), 1);
        glBindTexture(GL_TEXTURE_2D, gradient);
        }
        if (glGetUniformLocation(shader, "texture0") != -1) {
         glActiveTexture(GL_TEXTURE0);
            glUniform1i(glGetUniformLocation(shader, "texture0"), 0);
        glBindTexture(effect.textureTarget, effect.texture);
        }
        glDrawElementsInstanced(effect.drawTarget, elementsPerInstance, GL_UNSIGNED_INT, (void*)(batch->getIndexByteStride(effect.graphics.getVertexData())), effect.getNumParticles());
        glBindTexture(effect.textureTarget, 0);
       
    }
    glActiveTexture(0);
    glDepthMask(GL_TRUE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::print(std::string string) {
    screenText.duration = 3.7;
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
