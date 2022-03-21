//
//  MeshComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 11/5/21.
//

#include "MeshComponent.hpp"

MeshComponent::MeshComponent(Componentable& c) : Component(c) {
    mesh = std::shared_ptr<VertexMesh>();
    updatePriority = 4;
    meshAction = [] (VertexMesh* vm) {
        
    };
}

void MeshComponent::setMesh(std::shared_ptr<VertexMesh>& mesh_) {
    mesh = mesh_;
}

void MeshComponent::tick() {
    meshAction(mesh.get());
} 

void MeshComponent::setMeshAction(MeshAction &meshAction_) {
    meshAction = meshAction_;
}
