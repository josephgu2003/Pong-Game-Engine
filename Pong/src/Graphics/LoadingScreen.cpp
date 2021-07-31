//
//  LoadingScreen.cpp
//  Pong
//
//  Created by Joseph Gu on 6/12/21.
//

#include "LoadingScreen.hpp"

LoadingScreen::LoadingScreen() {
    frameShader = Shader("Shaders/LoadScreenV.vs", "Shaders/LoadScreenF.fs");
    textShader = Shader("Shaders/TextVShader.vs", "Shaders/TextFShader.fs");
      
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2000, 1300, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, 24*sizeof(float), &screenquad[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void LoadingScreen::loadTextData() {
    glGenVertexArrays(1, &tVAO);
    glBindVertexArray(tVAO);
    
    glGenBuffers(1, &tVBO);
    glBindBuffer(GL_ARRAY_BUFFER, tVBO);
    glBufferData(GL_ARRAY_BUFFER, screenText.textPosArray.size()*sizeof(float), screenText.textPosArray.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void LoadingScreen::print(std::string string) {
    screenText.duration = 0.5;
    screenText.text = string;
    if (screenText.Characters.size() == 0){
    AssetManager::loadGlyphs("Resources/Glyphs/times.ttf", screenText.Characters);
    }
    screenText.textPosArray = {};
    std::string::const_iterator c;
    float x = -0.9;
    float y = -0.8;
    float scale = 0.0004;
      for (c = screenText.text.begin(); c != screenText.text.end(); c++)
      {
          Character ch = screenText.Characters[*c];

          float xpos = x + ch.bearing.x * scale;
          float ypos = y - (ch.size.y - ch.bearing.y) * scale;

          float w = ch.size.x * scale;
        
          float h = ch.size.y * scale;
          
          // update VBO for each character
          std::vector<float> vertices = {
               xpos,     ypos + h,   0.0f, 0.0f ,
              xpos,     ypos,       0.0f, 1.0f ,
               xpos + w, ypos,       1.0f, 1.0f ,
               xpos,     ypos + h,   0.0f, 0.0f ,
               xpos + w, ypos,       1.0f, 1.0f ,
               xpos + w, ypos + h,   1.0f, 0.0f
          };
          screenText.textPosArray.insert(screenText.textPosArray.end(), vertices.begin(), vertices.end());
          x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
      }
    loadTextData();
    render();
}

void LoadingScreen::render() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
    textShader.use();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(tVAO);
    int verticeCount = 0;
    for (int i = 0; i < screenText.text.size(); i++) {
    glBindTexture(GL_TEXTURE_2D, screenText.Characters[screenText.text[i]].id);
    glDrawArrays(GL_TRIANGLES, verticeCount, 6);
    verticeCount += 6;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    frameShader.use();
    glBindVertexArray(VAO);
    glDisable(GL_DEPTH_TEST);
    
    glActiveTexture(GL_TEXTURE0); // accomodate more trextures later
    glUniform1i(glGetUniformLocation(frameShader.ID, "fbotexture"), 0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    screenText.duration -= glfwGetTime();
}
