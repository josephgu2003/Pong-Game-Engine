//
//  InputHandler.hpp
//  Pong
//
//  Created by Joseph Gu on 6/3/21.
//

#ifndef InputHandler_hpp
#define InputHandler_hpp

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>

class InputHandler{ //detects input, lets game know
    GLFWwindow* window;
public:
    InputHandler();
    ~InputHandler();
    void setWindow(GLFWwindow* window);
    void setKeyCallback(GLFWkeyfun func);
    void tick();
};

#endif /* InputHandler_hpp */
