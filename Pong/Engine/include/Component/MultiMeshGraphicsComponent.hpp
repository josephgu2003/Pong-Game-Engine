//
//  MultiMeshGraphicsComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 12/23/21.
//

#ifndef MultiMeshGraphicsComponent_hpp
#define MultiMeshGraphicsComponent_hpp

#include <stdio.h>
#include "GraphicsComponent.hpp"

class MultiMeshGraphicsComponent : public GraphicsComponent {
private:
    std::vector<GraphicsObject> meshes;
public:
    MultiMeshGraphicsComponent(Componentable& actor, Shader* shader, DrawPass dp);
    virtual ~MultiMeshGraphicsComponent();
    
    void initModel(const std::string& model) override;
    
    void addMesh(GraphicsObject go);
};

#endif /* MultiMeshGraphicsComponent_hpp */
