//
//  Fish.cpp
//  Pong
//
//  Created by Joseph Gu on 6/22/21.
//

#include "Fish.hpp"


Fish::Fish(World* world_, Actor* actor_, float duration_) : Ability(world_, actor_, duration_) {
    quad.pos = actor->getPos() + actor->dirVec + glm::vec3(0,1,0);
    quad.texture = loadTexture(TEX_PEACH);
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            Vertex vertex;
            vertex.Pos = glm::vec3( (float)(10*(-0.5f+i/49.0f)),(float)(-0.5f+j/49.0f),
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
    duration = 6;
    on = true;
}

Fish::~Fish() {
    world->deleteQuad(&quad);
}

void Fish::call(Game* game) {
    world->insertQuad(&quad);
    dirFired = actor->dirVec;
    mode2 = true;
    quad.force = 0.1f*dirFired;
}

void Fish::tick() {
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            if (i==0) {
            //quad.vertices.at((50*i+j)).Pos.z = 0.3*sin(20*(float)i/49.0f-2*duration);
                glm::vec3 forwardDir = quad.vertices.at(50*(i)+j).Pos - quad.vertices.at(50*(i+1)+j).Pos;
                glm::vec3 rightDir = glm::normalize(glm::cross(glm::vec3(0,1,0),forwardDir));
                glm::mat4 transform = glm::mat4(1.0f);
                float k = (float)(sin(duration)-sin(duration+0.1f))/(1+(sin(duration)*sin(duration+0.1f)));
                transform = glm::rotate(transform, glm::radians(0.3f*atan(20*k)), glm::vec3(0,1,0));
                transform = glm::translate(transform, 0.3f*forwardDir);
                
               // quad.vertices.at(50*(i)+j).Pos = glm::vec3( transform*glm::vec4(quad.vertices.at(50*(i)+j).Pos,1));
                
                quad.vertices.at((50*i+j)).Pos = glm::vec3(transform*glm::vec4(quad.vertices.at((50*i+j)).Pos,1));
                
               // glm::vec3 forwardDir = quad.vertices.at(50*(i)+j).Pos - quad.vertices.at(50*(i+1)+j).Pos;
             //   quad.vertices.at((50*i+j)).Pos += 0.1f*forwardDir;
           // quad.vertices.at((50*i+j)*5+2) += 0.3*sin(4*(float)j/49.0f-2*duration-1);
            } else {
                glm::vec3 lastVertex = quad.vertices.at(50*(i-1)+j).Pos;
                
                glm::vec3 dir = lastVertex - quad.vertices.at(50*i+j).Pos;
                float length = glm::length(dir);
                float k = 1.0f - 10.0f/49.0f/length;
                quad.vertices.at(50*i+j).Pos += k * dir;
            }
        }
        quad.pos = quad.vertices.at(0).Pos;
    }


    duration += glfwGetTime();
}

