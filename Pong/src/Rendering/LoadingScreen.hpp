//
//  LoadingScreen.hpp
//  Pong
//
//  Created by Joseph Gu on 6/12/21.
//
// draws the world


#ifndef LoadingScreen_hpp
#define LoadingScreen_hpp

#include <stdio.h>

//
//  Renderer.hpp
//  Pong
//
//  Created by Joseph Gu on 5/3/21.


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <vector>
#include "Shader.hpp"
#include "World.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include <string>

struct screentext {
    float duration;
    std::string text;
    std::map<char, Character> Characters;
    std::vector<float> textPosArray;
};


class LoadingScreen {
private:
    screentext screenText = {0, ""};
    
    GLuint VBO, VAO, tVAO, tVBO, fbo;
    
    float screenquad[24] =
    {   -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    
    GLuint texture = 0;

    Shader frameShader, textShader;

public:
    LoadingScreen();
 //   ~LoadingScreen();
    void loadTextData();
    void print(std::string string);
    void render();
};


#endif /* LoadingScreen_hpp */
