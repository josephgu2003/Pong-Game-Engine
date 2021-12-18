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
    std::weak_ptr<VertexMesh> mesh;
    DrawCall drawCall;
public:
    GraphicsComponent(Componentable& actor, Shader* shader, const Material& map, DrawPass dp);
    virtual ~GraphicsComponent();
    void initModel(const std::string& model); 
    void initGrid(int verticesX, int verticesY, float scale, std::shared_ptr<VertexMesh>& mesh);
    virtual void tick() override;
    virtual void draw(Renderer* r) override;
    void setDrawCall(DrawCall dc);
}; 
   

#endif /* GraphicsComponent_hpp */
