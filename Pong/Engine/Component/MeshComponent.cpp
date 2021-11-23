//
//  MeshComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 11/5/21.
//

#include "MeshComponent.hpp"

MeshComponent::MeshComponent(Componentable& c) : Component(c) {
    mesh = std::make_unique<VertexMesh>();
    type = ANIM;
    meshAction = [] (VertexMesh* vm) {
        
    };
}

void MeshComponent::setMesh(VertexMesh* mesh_) {
    mesh.reset(mesh_);
}

void MeshComponent::tick() {
    meshAction(mesh.get());
} 

void MeshComponent::setMeshAction(MeshAction &meshAction_) {
    meshAction = meshAction_;
}
