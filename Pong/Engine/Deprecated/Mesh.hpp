//
//  Mesh.hpp
//  Pong
//
//  Created by Joseph Gu on 5/14/21.
//

#ifndef Mesh_hpp
#define Mesh_hpp


class Billow;
/**
class Mesh : public VertexData {
    friend class Billow;
public: 
    Mesh();
    Mesh(std::vector<AnyVertex*>& vertices_,
         std::vector<GLuint>& indices_,
                TextureMaps& textures_, VertexType vertexType_);

    Mesh(const Mesh &vd) {
        indices = vd.indices;
        textures = vd.textures;
        vertexType = vd.vertexType;
        vertices.resize(vd.vertices.size());
        for (int i = 0; i < vd.vertices.size(); i++) {
            std::unique_ptr<AnyVertex> vertex = std::make_unique<AnyVertex>();
            *vertex.get() = *vd.vertices.at(i).get();
            vertices[i] = std::move(vertex);
        }
         
    }
     
    virtual void operator=(const Mesh& vd) {
        indices = vd.indices; 
        textures = vd.textures;
        vertexType = vd.vertexType;
        vertices.resize(vd.vertices.size());
        for (int i = 0; i < vd.vertices.size(); i++) {
            std::unique_ptr<AnyVertex> vertex = std::make_unique<AnyVertex>();
            *vertex.get() = *vd.vertices.at(i).get();
            vertices[i] = std::move(vertex);
        }
         
    } 
    
};**/
#endif /* Mesh_hpp */
