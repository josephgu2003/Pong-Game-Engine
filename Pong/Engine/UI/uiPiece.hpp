//
//  uiPiece.hpp
//  Pong
//
//  Created by Joseph Gu on 6/21/21.
//

#ifndef uiPiece_hpp
#define uiPiece_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include "GraphicsObject.hpp" 

class uiPiece : public GraphicsObject { // maybe do position relative in constructor, store real position internally?
protected:
    std::vector<std::shared_ptr<uiPiece>> children;
    glm::vec2 position;
    glm::vec2 dimensions;
public:
    uiPiece(glm::vec2 position, glm::vec2 dimensions, std::string vs, std::string fs);
    virtual void draw(Renderer* r) override;
    void insertChild(const std::shared_ptr<uiPiece>& uip);
};

#endif /* uiPiece_hpp */
