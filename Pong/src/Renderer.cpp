//
//  Renderer.cpp
//  Pong
//
//  Created by Joseph Gu on 5/3/21.
//

#include "Renderer.hpp"

Renderer::Renderer() {
    actorShader = new Shader("Shaders/ActorVertexShader.vs", "Shaders/ActorFragmentShader.fs"); // make own function later
    mapShader = new Shader("Shaders/MapVertexShader.vs", "Shaders/MapFragmentShader.fs");
    skyShader = new Shader("Shaders/SkyVertexShader.vs", "Shaders/SkyFragmentShader.fs");
    modelMat = glm::mat4(1);
    viewMat = glm::mat4(1);
    projMat = glm::perspective(glm::radians(50.0f), 1000.0f/800.0f, 0.01f, 100.0f);
}

Renderer::~Renderer() {
    
}

void Renderer::setWorld(World *world_) {
    Renderer::world = world_;
}

void Renderer::setCamera(Camera *camera_) {
    camera = camera_;
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
    numActorTriangles = actorVertices.size();
   // world->loadVerticesIndices();
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
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
}

void Renderer::loadMapData() {
    int imageWidth, imageHeight, channels;
    unsigned char* imageData = stbi_load("Resources/Map/ice.jpeg", &imageWidth, &imageHeight, &channels, 0);
    
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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
    glBindVertexArray(0);
}
void Renderer::loadSkyBoxData() {
    int imageWidth, imageHeight, channels;
    unsigned char* imageData;
    glGenTextures(1, &skyTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture);
    std::vector<std::string>* skyFiles = world->getSkyTextureFiles();
    for (unsigned int i = 0; i < skyFiles->size(); i++) {
        imageData = stbi_load(skyFiles->at(i).c_str(), &imageWidth, &imageHeight, &channels, 0);
        if (imageData) {
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
void Renderer::render() {
    // actors
    glBindVertexArray(VAO);
    actorShader->use();
    camera->actor->updateVecs();
    camera->actor->orient(camera->getYaw());
    camera->updateVecs();
    viewMat = glm::lookAt(camera->posVec,camera->dirVec+camera->posVec, glm::vec3(0.0,1.0,0.0));
    glfwSetTime(0);
    int indiceCount = 0;
    for (int i = 0; i<world->getActorsCount(); i++) {
    loadModelMatrix(i);
    std::vector<Mesh>* meshes = world->getNthActor(i)->model->getMeshes();
        for (int j = 0; j < meshes->size(); j++) {
            GLuint id1 = meshes->at(j).textures.at(0).id;
            GLuint id2 = meshes->at(j).textures.at(1).id;
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(glGetUniformLocation(actorShader->ID, "texture0"), 0);
            glBindTexture(GL_TEXTURE_2D, id1);
            
            glActiveTexture(GL_TEXTURE1);
            glUniform1i(glGetUniformLocation(actorShader->ID, "texture1"), 1);
            glBindTexture(GL_TEXTURE_2D, id2);
          
            glUniformMatrix4fv(glGetUniformLocation(actorShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
            glUniformMatrix4fv(glGetUniformLocation(actorShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
            glUniformMatrix4fv(glGetUniformLocation(actorShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
            glActiveTexture(GL_TEXTURE0);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            glDrawElements(GL_TRIANGLES, meshes->at(j).indices.size(), GL_UNSIGNED_INT, (void*) indiceCount);
            
            indiceCount += (meshes->at(j).indices.size())*sizeof(GLuint);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    //map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    mapShader->use();
    modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, glm::vec3(0.0f, -0.125f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(mapShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix4fv(glGetUniformLocation(mapShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
    glUniformMatrix4fv(glGetUniformLocation(mapShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //sky
    glDepthMask(GL_FALSE);
    skyShader->use();
    viewMat = glm::mat4(glm::mat3(viewMat));
    glUniformMatrix4fv(glGetUniformLocation(skyShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
    glUniformMatrix4fv(glGetUniformLocation(skyShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(sVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sVBO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}

void Renderer::loadModelMatrix(int n) {
    modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, world->getNthActor(n)->getPos());
    modelMat = glm::rotate(modelMat, -glm::radians(-90+world->getNthActor(n)->getYaw()), glm::vec3(0,1,0));
}
