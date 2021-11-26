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
#include "GraphicsObject.hpp"
#include "Component.hpp"
#include "VertexMesh.hpp"
  
class Shader;

struct FrameAndShader { 
    Frame* frame = NULL;
    Shader* shader = NULL;
};

class Componentable;

class GraphicsComponent;

typedef void (*DrawCall) (Renderer*, GraphicsComponent*);

class GraphicsComponent : public Component, public GraphicsObject { // for actors
protected:
    std::map<Texture*, FrameAndShader> animatedTextures;
    VertexMesh* mesh = nullptr;
    DrawCall drawCall;
public:  
    GraphicsComponent(Componentable& actor, Shader* shader, const Material& map);
    void initModel(const std::string& model); 
    void initGrid(int verticesX, int verticesY, float scale, VertexMesh*& mesh);
    virtual void tick() override;
    virtual void draw(Renderer* r) override;
    void setDrawCall(DrawCall dc);
    void animateTexture(Texture* texture, Shader* shader);
};
 

#endif /* GraphicsComponent_hpp */
