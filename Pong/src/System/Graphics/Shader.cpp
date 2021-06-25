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

Shader::Shader () {
    
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    //get code from file location
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    //files and exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        //read into streams
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close ifstreams
        vShaderFile.close();
        fShaderFile.close();
        // stream to string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    //put into const char*
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    
    //compile shaders
    GLuint vShader, fShader;
    int success;
    char errorLog [512];
    
    vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vShaderCode, NULL);
    glCompileShader(vShader);
    
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(vShader, 512, NULL, errorLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << errorLog << std::endl;
    }
    
    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fShaderCode, NULL);
    glCompileShader(fShader);
    
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(fShader, 512, NULL, errorLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << errorLog << std::endl;
    }
    
    //make program
    Shader::ID = glCreateProgram();
    glAttachShader(ID, vShader);
    glAttachShader(ID, fShader);
    glLinkProgram(ID);
    
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, errorLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << errorLog << std::endl;
    }
    
    glDeleteShader(vShader);
    glDeleteShader(fShader);

}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::init(const char *vertexPath, const char *fragmentPath) {
    //get code from file location
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    //files and exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        //read into streams
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close ifstreams
        vShaderFile.close();
        fShaderFile.close();
        // stream to string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    //put into const char*
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    
    //compile shaders
    GLuint vShader, fShader;
    int success;
    char errorLog [512];
    
    vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vShaderCode, NULL);
    glCompileShader(vShader);
    
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(vShader, 512, NULL, errorLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << errorLog << std::endl;
    }
    
    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fShaderCode, NULL);
    glCompileShader(fShader);
    
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(fShader, 512, NULL, errorLog);
        std::string error(fragmentPath);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << errorLog << error<< std::endl;
    }
    
    //make program
    Shader::ID = glCreateProgram();
    glAttachShader(ID, vShader);
    glAttachShader(ID, fShader);
    glLinkProgram(ID);
    
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, errorLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << errorLog << std::endl;
    }
    
    glDeleteShader(vShader);
    glDeleteShader(fShader);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, glm::vec3 vector) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vector));
}

void Shader::setMat4(const std::string &name, glm::mat4 matrix) {
    glm::mat4 test = matrix;
    use();
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(test));
}


