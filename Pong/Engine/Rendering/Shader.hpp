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
#include <unordered_map>
 
#define FUNCTION_DEFS "Shaders/Functions.json"
// manages a shader
class Shader
{
private:
    void fillInFunctions(std::string& shaderCode);
    static
    std::unordered_map<std::string, std::string> functions;
    void makeShader(GLenum shaderType, const char* src, GLuint& shader);
public:
    // program ID
    GLuint ID;
    // constructor takes source files to build shader
    Shader();
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
    
    //use the shader
    static void loadFunctionDefinitions();
    void use();
    void init(const char* vertexPath, const char* fragmentPath);
    void init(const char* vertexPath,const char* geoPath, const char* fragmentPath);
    
    //set uniform variables
    void setUniform(const std::string &name, bool value) const;
    void setUniform(const std::string &name, int value) const;
    void setUniform(const std::string &name, float value) const;
    void setUniform(const std::string &name, double value) const;
    void setUniform(const std::string &name, const glm::vec3& vector) const;
    void setUniform(const std::string &name, const glm::vec4& vector) const;
    void setUniform(const std::string &name, glm::mat4& matrix);
    void setUniform(const std::string &name, glm::mat3& matrix);
};

#endif /* Shader_hpp */
