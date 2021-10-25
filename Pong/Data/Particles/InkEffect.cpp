//
//  InkGlyphs.cpp
//  Pong
//
//  Created by Joseph Gu on 6/13/21.
//

#include "InkEffect.hpp"
#include "VertexData.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>


void InkEffect::setGraphics(Shader* shader) {
    /**load3DTexture("Resources/Particles/Smokes/smoke000.jpg", texture);
    drawTarget = GL_POINTS;
     textureTarget = GL_TEXTURE_2D_ARRAY;
     shader.init("Shaders/SmokeVShader.vs", "Shaders/SmokeFShader.fs");  
    shader.use();
    shader.setFloat("size", size);**/
/**    AssetManager::loadTexture(TEX_GRADIENT, &texture, false);
    textureTarget = GL_TEXTURE_2D;
    drawTarget = GL_POINTS;  
    shader->init("Shaders/ColorPartV.vs", "Shaders/ColorPartF.fs");
    shader->use();
    glm::vec4 color = glm::vec4(15.0,4.2,2.4,1.0);  
    shader->setVec4("color", color);
    shader->setFloat("size", size); **/
}

void InkEffect::refreshParticle() {
   glm::vec3 newPos = glm::vec3(0.0);

  /**  int count = 0;
    
    for (int i = 0; i < model->getMeshes()->size(); i++) {
        count += model->getMeshes()->at(i).getVertices().size();
    }
    
    int v = 0.01*count*(distribution(generator)%100);
    int count2 = 0;
    
    for (int i = 0; i < model->getMeshes()->size(); i++) {
        if (v >= model->getMeshes()->at(i).getVertices().size()+count2) {
            count2 += model->getMeshes()->at(i).getVertices().size();
        } else {
            newPos =  static_cast<TBNVertex*>(model-> getMeshes()->at(i).getVertices().at(v-count2).get())->Pos;
            glm::mat3 modelMat = glm::mat3(1.0f);
            modelMat = glm::rotate(modelMat, glm::radians(-90.0f+actor->getYaw()));
            glm::vec3 vec = glm::vec3(newPos.x, newPos.z, 1.0); 
            vec = modelMat * vec;
            newPos.x = vec.x;
            newPos.z = vec.y;
        } 
    }**/ 
 /**
particles[firstUnused].posVec = 0.005f*newPos+actor->getPos();
    float a = 0.0005*(distribution(generator)%100);
    float b = 0.0005*(distribution(generator)%100);
    float c = 0.0005*(distribution(generator)%100);

particles[firstUnused].velVec = (glm::vec3(a-0.025,b-0.025,c-0.025));
  
particles[firstUnused].duration = 5.0f;
particles[firstUnused].texture = texture.id;**/
}

void InkEffect::setActor(Actor *actor_) {
   // actor = actor_;
//    model = actor->getComponent<GraphicsComponent>()->getModel();
}

