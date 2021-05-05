//
//  Renderer.cpp
//  Pong
//
//  Created by Joseph Gu on 5/3/21.
//

#include "Renderer.hpp"

double lastMX = 300;
double lastMY = 300;
float xOffset;
float yOffset;

void mouse_callback(GLFWwindow* window, double mouseX_, double mouseY_) {
    xOffset = mouseX_ - lastMX;
    yOffset = lastMY - mouseY_;
    lastMX = mouseX_;
    lastMY = mouseY_;
}

Renderer::Renderer(World *world_, GLFWwindow* window_) {
    Renderer::world = world_;
    Renderer::window = window_;
    std::cout << world << "\n";
    shader = new Shader("/Users/josephgu/Documents/Code/Pong/Pong/VertexShaderCode.vs", "/Users/josephgu/Documents/Code/Pong/Pong/FragmentShaderCode.fs"); // make own function later
    modelMat = glm::mat4(1);
    viewMat = glm::mat4(1);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
}

Renderer::~Renderer() {
    
}

void Renderer::loadVertices() {
    GLuint VBO = 0;
    GLuint VAO = 0;
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    std::cout << world << "\n";
    float* test = (world->getVertices());
    glBufferData(GL_ARRAY_BUFFER, (world->getVerticesCount()) * sizeof(float), test, GL_STATIC_DRAW);
    
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
}

void Renderer::render() {
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    shader->use();
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.incX(-0.01f);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.incX(0.01f);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.incZ(0.01f);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.incZ(-0.01f);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.incY(0.01f);
    }
    if ((glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS) && (camera.getY()>0.0f)) {
        camera.incY(-0.01f);
    }
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        std::cout << camera.getX() << camera.getY() << camera.getZ() << camera.getYaw() << camera.getPitch() << "\n";
    }
    findMatrices(1);
    glUniform4f(glGetUniformLocation(shader->ID, "aColor"), 1.0f, 0.5f, 0.6f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));

    glDrawArrays(GL_TRIANGLES, 0, 72);
                                        
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glUniform4f(glGetUniformLocation(shader->ID, "aColor"), 0.5f, 0.2f, 0.2f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 72);
}

void Renderer::findMatrices(int n) {
    camera.incYaw(xOffset*0.05);
    camera.incPitch(yOffset*0.05);
    // modelspace -> worldspace, model matrix
    modelMat = glm::mat4(1.0f);
    viewMat = glm::mat4(1.0f);
    float x, y, z;
    x = world->getNthActor(n)->getX();
    y = world->getNthActor(n)->getY();
    z = world->getNthActor(n)->getZ();
 //   modelMat = glm::translate(modelMat, glm::vec3(x, y, z));
//    modelMat = glm::rotate(modelMat,glm::radians(50*(float)glfwGetTime()), glm::vec3(1.0f,0.0f,0.0f));
 //   modelMat = glm::rotate(modelMat,glm::radians(50*(float)glfwGetTime()), glm::vec3(0.0f,1.0f,0.0f));
    
    //worldspace->viewspace
    float camOrientX, camOrientY, camOrientZ;
    camOrientX = std::cos(glm::radians(camera.getYaw())) * std::cos(glm::radians(camera.getPitch()));
    camOrientY = std::sin(glm::radians(camera.getPitch()));
    camOrientZ = std::sin(glm::radians(camera.getYaw())) * std::cos(glm::radians(camera.getPitch()));
    viewMat = glm::lookAt(glm::vec3(camera.getX(),camera.getY(),camera.getZ()),glm::vec3(camOrientX+camera.getX(),camOrientY+camera.getY(),camOrientZ+camera.getZ()), glm::vec3(0.0,1.0,0.0));
    
    //viewspace -> clipspace, near vs far looks different after
        projMat = glm::perspective(glm::radians(50.0f), 1.0f, 0.1f, 100.0f);

}
