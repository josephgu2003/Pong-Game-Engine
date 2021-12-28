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
     
public:
    MultiMeshGraphicsComponent(Componentable& actor, Shader* shader, DrawPass dp);
    virtual ~MultiMeshGraphicsComponent();
    
    void initModel(const std::string& model) override;
};

#endif /* MultiMeshGraphicsComponent_hpp */
