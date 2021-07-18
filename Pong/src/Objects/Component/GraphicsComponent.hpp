//
//  GraphicsComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 6/27/21.
//

#ifndef GraphicsComponent_hpp
#define GraphicsComponent_hpp

#include "Component.hpp"
#include "Shader.hpp"
#include "VertexData.hpp"
#include <memory>

class GraphicsComponent : public Component {
protected:
    Shader* shader;
    std::unique_ptr<VertexData> vertexData;
    int activeData = -1;
    
public:
    GraphicsComponent();
    
    GraphicsComponent(VertexData* vertexData_, Shader* shader_);

    void init(VertexData* vertexData_, Shader* shader_);
    
    virtual void tick(Actor& actor, World& world) override;
    
    VertexData* getVertexData();

    Shader* getShader();
};


#endif /* GraphicsComponent_hpp */
