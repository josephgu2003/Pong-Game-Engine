//
//  Shader.hpp
//  Pong
//
//  Created by Joseph Gu on 4/30/21.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    // program ID
    GLuint ID;
    
    // constructor takes source files to build shader
    Shader(const char* vertexPath, const char* fragmentPath);
    
    //use the shader
    void use();
    
    //set uniform variables
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
};

#endif /* Shader_hpp */
