//
//  GraphicsComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 6/27/21.
//

#ifndef GraphicsComponent_hpp
#define GraphicsComponent_hpp

#include "Component.hpp"
#include <memory> 
#include "AssetManager.hpp"
#include "Renderable.hpp"
#include "ActorComp.hpp"
  
class Shader;

struct FrameAndShader {
    Frame* frame = NULL;
    Shader* shader = NULL;
};

class GraphicsComponent : public ActorComp, public Renderable { // for actors
protected:
    std::map<Texture*, FrameAndShader> animatedTextures;
public:  
    GraphicsComponent(Actor& actor);
    void init(Shader* shader, const std::string& model, const TextureMaps& map);
    virtual void tick() override;
    virtual void draw(Renderer* r) override;

    void animateTexture(Texture* texture, Shader* shader);
};


#endif /* GraphicsComponent_hpp */
