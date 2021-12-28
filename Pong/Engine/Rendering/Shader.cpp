//
//  Shader.cpp
//  Pong
//
//  Created by Joseph Gu on 4/30/21.
//

#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Actor.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "json.hpp"

std::unordered_map<std::string, std::string> Shader::functions;
 
void Shader::loadFunctionDefinitions() {
    nlohmann::json jFunctions;
    std::ifstream i(FUNCTION_DEFS);
    i >> jFunctions;
    functions = jFunctions.get<std::unordered_map<std::string, std::string>>();
}

Shader::Shader () {
    
}

void Shader::fillInFunctions(std::string &shaderCode) {
    for (auto funcDefinition = functions.begin(); funcDefinition != functions.end(); funcDefinition++) {
        std::string symbol = funcDefinition->first;
        size_t found = shaderCode.find(symbol);
        while (found != std::string::npos) {
            std::string inlinedFunctionCode = funcDefinition->second;
            size_t terminationSymbol = shaderCode.find(";", found);
            shaderCode.replace(found, 1+terminationSymbol - found, inlinedFunctionCode);
            found = shaderCode.find(symbol);  
        }
    }
}
       
  
void Shader::makeShader(GLenum shaderType, const char* src, GLuint& shader) {
    std::string shaderCode;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(src);
        std::stringstream tempStream;
        tempStream << shaderFile.rdbuf(); 
        shaderFile.close();
        shaderCode = tempStream.str();
    }
    catch(std::ifstream::failure e) {
        printf("Shader file not successfully read : %s \n", src);
    }
    
    fillInFunctions(shaderCode);
    const char* shaderCode_ = shaderCode.c_str();
    
    int success;
    char errorLog[512];
    
    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode_, NULL);
    glCompileShader(shader);
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        glGetProgramInfoLog(shader, 512, NULL, errorLog);
        printf("Shader not compiled : %s ", src);
        printf("%s \n", errorLog);
    }
}
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    init(vertexPath, fragmentPath);
}
 
void Shader::use()
{
    glUseProgram(ID);
}

void Shader::init(const char *vertexPath, const char *fragmentPath) {
    //compile shaders
    GLuint vShader, fShader;

    makeShader(GL_VERTEX_SHADER, vertexPath, vShader);
    makeShader(GL_FRAGMENT_SHADER, fragmentPath, fShader);

    //make program
    Shader::ID = glCreateProgram();
    glAttachShader(ID, vShader);
    glAttachShader(ID, fShader);
    glLinkProgram(ID);
    
    int success;
    char errorLog[512];
    
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, errorLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << errorLog << std::endl;
    }
    
    glDeleteShader(vShader);
    glDeleteShader(fShader);
}

void Shader::init(const char* vertexPath,const char* geoPath, const char* fragmentPath) {
    GLuint vShader, fShader, gShader;

    makeShader(GL_VERTEX_SHADER, vertexPath, vShader);
    makeShader(GL_FRAGMENT_SHADER, fragmentPath, fShader);
    makeShader(GL_GEOMETRY_SHADER, geoPath, gShader);
    //make program
    Shader::ID = glCreateProgram();
    glAttachShader(ID, vShader);
    glAttachShader(ID, fShader);
    glAttachShader(ID, gShader);
    glLinkProgram(ID);
    
    int success;
    char errorLog[512];
    
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, errorLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << errorLog << std::endl;
    }
    
    glDeleteShader(vShader);
    glDeleteShader(fShader);
    glDeleteShader(gShader);
}

void Shader::setUniform(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}   

void Shader::setUniform(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setUniform(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setUniform(const std::string &name, double value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), float(value));
} 
 
 
void Shader::setUniform(const std::string &name, const glm::vec3& vector) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vector));
} 
 
void Shader::setUniform(const std::string &name, const glm::vec4& vector) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vector));
} 

void Shader::setUniform(const std::string &name, glm::mat4& matrix) {
    use();
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniform(const std::string &name, glm::mat3& matrix) {
    use();
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

// uniforms: global uniform objects, on renderable tick uniforms, on instance uniforms
Shader::~Shader() {
    glDeleteShader(ID);
}
 
