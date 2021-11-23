//
//  MeshComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 11/5/21.
//

#ifndef MeshComponent_hpp
#define MeshComponent_hpp

#include <stdio.h>
#include "VertexMesh.hpp"
#include "Component.hpp"
#include <functional> 

class Componentable;

typedef std::function<void (VertexMesh*)> MeshAction; 

class MeshComponent : public Component {
private:
    std::function<void (VertexMesh*)> meshAction; // problem : this depends on many factors
    // fishy pattern
    // waving flag
    //
    std::unique_ptr<VertexMesh> mesh;
public:
    MeshComponent(Componentable& c);
    void setMesh(VertexMesh* mesh);
    virtual void tick() override;
    void setMeshAction(MeshAction& meshAction_); // spag
}; 
 
#endif /* MeshComponent_hpp */
