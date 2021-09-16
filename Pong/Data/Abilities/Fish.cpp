//
//  Fish.cpp
//  Pong
//
//  Created by Joseph Gu on 6/22/21.
//

#include "Fish.hpp"
#include "Game.hpp"
#include "CombatComponent.hpp"

Fish::Fish(World* world_, Actor* actor_, float duration_) : Ability(world_, actor_, duration_) {
    force = new Force();
    force->init(glm::vec3(0,0,0), -0.2);
    force->configureSuctionForce(0.5, FORCE_LINEAR);
    quad.pos = actor->getPos() + actor->dirVec + glm::vec3(0,1,0);
    AssetManager::loadTexture(TEX_FISH, &quad.texture, true);
    quad.alpha = 0.0; 
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            Vertex vertex;
            vertex.Pos = glm::vec3( (float)(2.0f*(-0.5f+i/49.0f)),2.0f*(float)(-0.5f+j/49.0f),
                                         (0.0f));
            vertex.Pos += quad.pos;
           vertex.Normal = glm::vec3(0,0,1);
     
            vertex.TexCoords = glm::vec2( (float)(0.0f+i/49.0f), (float)(0.0f+j/49.0f));
            
            quad.vertices.push_back(vertex);
        }
    }
        
    std::vector<GLuint> indices;
    for (int i = 0; i < 49; i++) {
        for (int j = 0; j < 49; j++) {
        quad.indices.push_back(0.0f+j+50*i);
        quad.indices.push_back(0.0f+j+50*i+1);
        quad.indices.push_back(0.0f+j+50*i+50);
        }
    }
    for (int i = 0; i < 49; i++) {
        for (int j = 0; j < 49; j++) {
        quad.indices.push_back(0.0f+j+50*i+1);
        quad.indices.push_back(0.0f+j+50*i+50);
        quad.indices.push_back(0.0f+j+50*i+50+1);
        }
    }
    quad.size = 3;
    quad.rotations = glm::vec3(0,-90+actor->getYaw(),0);
    quad.force = glm::vec3(0,0,0);
    duration = duration_;
    on = true;
}

Fish::~Fish() {
    world->deleteQuad(&quad);
    world->deleteForce(force);
}

void Fish::call() {
    world->insertQuad(&quad);
    world->insertForce(force);
    dirFired = actor->dirVec;
    mode2 = true;
    quad.force = 0.1f*dirFired;
}
 
void Fish::tick() {
    force->setPos(quad.pos);
    if (step == 0) {
        if (duration > 16.5) {
            quad.alpha = (18.0-duration)/1.5;
        }
        else {
            step++;
            freeActor();
        }
    }
     
    if (step == 1) {
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            if (i==0) {
                if (j ==0) {
                glm::vec3 forwardDir = normalize(quad.vertices.at(50*(i)+j).Pos - quad.vertices.at(50*(i+1)+j).Pos);
                
                glm::mat4 transform = glm::mat4(1.0f);
                
                float k = (float)(sin(duration*3.0f)-sin(duration*3.0f+0.1f))/(1.0f+(sin(duration*3.0f)*sin(duration*3.0f+0.1f)));
                
                     transform = glm::rotate(transform, glm::radians(100.0f*atan(k)), glm::vec3(0,1,0));
                
                float l = (float)(sin(duration*30.0f)-sin(duration*30.0f+0.1f))/(1.0f+(sin(duration*30.0f)*sin(duration*30.0f+0.1f)));
                
                     transform = glm::rotate(transform, glm::radians(0.8f*atan(k)+0.2f*atan(l)+2.0f), glm::vec3(0,1,0));
        
                forwardDir *= 0.016f;
                forwardDir = glm::vec3(transform*glm::vec4(forwardDir,1));
        
                    for (int j2 = 0 ; j2 < 50; j2++) {
                quad.vertices.at((50*i+j2)).Pos += forwardDir;
                    }
                }
            } else {
                glm::vec3 lastVertex = quad.vertices.at(50*(i-1)+j).Pos;
                
                glm::vec3 dir = lastVertex - quad.vertices.at(50*i+j).Pos;
                float length = glm::length(dir);
     float p = 1.0f - 2.0f/49.0f/length;
                quad.vertices.at(50*i+j).Pos += p * dir;
            }
        }
        quad.pos = quad.vertices.at(0).Pos;
    }
    }
    if (duration < 0) {
        on = false;
    }
    duration -= (float)glfwGetTime();
}

void Fish::freeActor() {
    if (actor->getComponent<CombatComponent>()->getAffecting().get() != NULL) {
       actor->getComponent<CombatComponent>()->getAffecting()->dispel();
    }
}
