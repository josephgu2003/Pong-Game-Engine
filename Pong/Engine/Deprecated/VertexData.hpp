//
//  VertexData.hpp
//  Pong
//
//  Created by Joseph Gu on 7/14/21.
//

#ifndef VertexData_hpp
#define VertexData_hpp



//#define MAX_BONE_WEIGHTS 4



enum VertexTemplate {
    QUAD_SIMPLE
};
/**
class VertexData {
protected:
    std::vector<std::unique_ptr<AnyVertex>> vertices;
    std::vector<GLuint> indices; 
    TextureMaps textures;
    VertexType vertexType;
public:
    VertexData(VertexTemplate vt);
    
    VertexData();
    
    VertexData(std::vector<AnyVertex*>& vertices_,
         std::vector<GLuint>& indices_,
               TextureMaps& textures_, VertexType vertexType_);
    
    VertexData(const std::string& text);
    
    VertexData(const VertexData &vd) {
        printf("VertexData copy constructor called \n");
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
           
    void operator=(const VertexData& vd) {
        printf("VertexData = overload called \n");
        indices = vd.indices; 
        textures = vd.textures;
        vertexType = vd.vertexType; 
    //    vertices.resize(vd.vertices.size());
        for (int i = 0; i < vd.vertices.size(); i++) {
          //  printf("%f", (static_cast<SimpleVertex*>(vd.vertices.at(i).get()))->Pos.x);
           // printf("%f", (static_cast<SimpleVertex*>(vd.vertices.at(i).get()))->Pos.y);
            SimpleVertex* v = static_cast<SimpleVertex*>(vd.vertices.at(i).get());
            glm::vec3 pos = v->Pos; 
            glm::vec2 tc = v->TexCoords;
            float id = v->arraytexID;
            std::unique_ptr<AnyVertex> vertex = std::make_unique<SimpleVertex>(pos, tc, id);
     
            vertices.push_back(std::move(vertex)); 
         //   printf("%f", (static_cast<SimpleVertex*>(vertices.back().get()))->Pos.x);
         //   printf("%f \n", (static_cast<SimpleVertex*>(vertices.back().get()))->Pos.y);
        }
    }
     
    // Joseph Gu, Northeastern University, 9/20/2021
    
    void setVertexData(std::vector<AnyVertex*>& vertices_,
                 std::vector<GLuint>& indices_,
                       TextureMaps& textures_, VertexType vertexType_);
    void generateTemplate(VertexTemplate vt);
    
    const std::vector<std::unique_ptr<AnyVertex>>& getVertices();
    void setVertices(std::vector<AnyVertex*>& vertices_, VertexType vertexType_);
    
    std::vector<GLuint>& getIndices();
    void setIndices(std::vector<GLuint>& indices_);
    
    TextureMaps& getTextures();
    void setTextures(TextureMaps& textures_);
    
};**/

#endif /* VertexData_hpp */
