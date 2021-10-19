//
//  Map.hpp
//  Pong
//
//  Created by Joseph Gu on 6/24/21.
//

#ifndef Map_hpp
#define Map_hpp 

#include <stdio.h>
#include <glm/glm.hpp>
#include "Positionable.hpp"
 
class GraphicsComponent;

class MapObject : public Positionable {
    GraphicsComponent* graphics = NULL;
public:
    void init();
    void init(glm::vec3 pos_);
    void setGraphics(GraphicsComponent* graphics);
    GraphicsComponent& getGraphics();
    void beginAnimation();
    void tick();
};
#endif /* Map_hpp */
