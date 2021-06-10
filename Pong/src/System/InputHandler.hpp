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

class InputHandler{
    GLFWwindow* window;
public:
    InputHandler();
    ~InputHandler();
  // void onetap_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
   // void mouse_callback(GLFWwindow* window, double mouseX_, double mouseY_);
    void setWindow(GLFWwindow* window);
    void setKeyCallback(GLFWkeyfun func);
    void setCharCallback(GLFWcharfun func);
    void tick();
};

#endif /* InputHandler_hpp */
