//
//  Sketch.cpp
//  Pong
//
//  Created by Joseph Gu on 6/8/21.
//

#include "Sketch.hpp"

Sketch::Sketch(World* world_, Actor* actor_, float duration_, GLuint texture_) : Ability(world_, actor_, duration_) {
    quad.pos = actor->getPos() + 3.0f*actor->dirVec + glm::vec3(0,3.0,0);
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            Vertex vertex;
            vertex.Pos = glm::vec3( (float)(3.0*(-0.5f+i/49.0f)),(float)(3.0*(-0.5f+j/49.0f)),
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
    quad.texture.id = texture_;
    quad.rotations = glm::vec3(0,-90+actor->getYaw(),0);
    quad.force = glm::vec3(0,0,0);
    duration = 6;
    quad.alpha = 1.0;
    on = true;
}

Sketch::~Sketch() {
    world->deleteQuad(&quad);
}

void Sketch::call(Game* game) {
    world->insertQuad(&quad);
}

void Sketch::tick() {
 /**   for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
           
            }
        }
        quad.pos = quad.vertices.at(0).Pos;**/


    if (mode2 == true) {
        float deltat = glfwGetTime();
        if(duration<20) {
            for (int i = 0; i < 50; i++) {
                   for (int j = 0; j < 50; j++) {
                       quad.vertices.at(50.0*i+j).Pos += 2*deltat * dirFired;
                       }
                   }
            quad.pos += 2*deltat * dirFired;
           // quad.rotations.y += deltat*20;
       //     world->setWeather(true, false, quad.rotations.x/240+0.2);
        }
    //    if(quad.rotations.x >= 90) {
     //       quad.rotations.y += deltat*-40;
     //   }
    }
    //0.73833
    duration += glfwGetTime();
}

void Sketch::call2() {
    dirFired = actor->dirVec;
    mode2 = true;
    quad.force = 0.1f*dirFired;
   /** for (int i = 0; i < 2500; i++) {
        quad.vertices.at(5*i) *= 0.767165;
       // quad.vertices.at(5*i+1) *= 0.767165;
    }**/
}
