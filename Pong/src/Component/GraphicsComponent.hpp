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
#include "AssetManager.hpp"

struct FrameAndShader {
    Frame* frame;
    Shader* shader;
};
class GraphicsComponent : public Component {
protected:
    Shader* shader = NULL;
    std::unique_ptr<VertexData> vertexData;
    Model* model = NULL;
    std::map<Texture*, FrameAndShader> animatedTextures;
    int activeData = -1;
public:
    GraphicsComponent();
    
    GraphicsComponent(VertexData* vertexData_, Shader* shader_);
    
    void setModel(Model* model);

    void init(VertexData* vertexData_, Shader* shader_);
    
    virtual void tick() override;
    
    VertexData* getVertexData();
    
    Model* getModel();

    Shader* getShader();
    
    void animateTexture(Texture* texture, Shader* shader);
};


#endif /* GraphicsComponent_hpp */
