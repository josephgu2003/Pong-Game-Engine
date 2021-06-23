//
//  Shader.hpp
//  Pong
//
//  Created by Joseph Gu on 4/30/21.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>

// manages a shader
class Actor;
class Renderer;

class Shader
{
public:
    // program ID
    GLuint ID;
    
    // constructor takes source files to build shader
    Shader();
    Shader(const char* vertexPath, const char* fragmentPath);
    
    //use the shader
    void use();
    void init(const char* vertexPath, const char* fragmentPath);
    
    //set uniform variables
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, glm::vec3 vector) const;
    void setMat4(const std::string &name, glm::mat4 matrix) const;
    
    void getRenderableData(Actor& actor);
};

#endif /* Shader_hpp */
