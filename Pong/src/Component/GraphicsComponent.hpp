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
#include "Renderable.hpp"

struct FrameAndShader {
    Frame* frame = NULL;
    Shader* shader = NULL;
};

class GraphicsComponent : public Component, public Renderable { // for actors
protected:
    std::map<Texture*, FrameAndShader> animatedTextures;
public:  
    GraphicsComponent();
    
    void init(Shader* shader, const std::string& model, const TextureMaps& map);
    virtual void tick() override;
    virtual void draw(Renderer* r) override;

    void animateTexture(Texture* texture, Shader* shader);
};


#endif /* GraphicsComponent_hpp */
