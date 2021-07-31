//
//  Map.hpp
//  Pong
//
//  Created by Joseph Gu on 6/24/21.
//

#ifndef Map_hpp
#define Map_hpp

#include <stdio.h>
#include "Shader.hpp"
#include "VertexData.hpp"
#include "Mesh.hpp"
#include <glm/glm.hpp>
#include "GraphicsComponent.hpp"

class MapObject {
    GraphicsComponent* graphics = NULL;
    glm::vec3 pos;
public:
    void init();
    void init(glm::vec3 pos_);
    void setPos(glm::vec3 pos_);
    glm::vec3 getPos();
    void setGraphics(GraphicsComponent* graphics);
    GraphicsComponent& getGraphics();
    void beginAnimation();
    void tick();
};
#endif /* Map_hpp */
