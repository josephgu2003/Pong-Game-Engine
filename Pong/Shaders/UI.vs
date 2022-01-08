#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 TexCoords_;
layout (location = 2) in float arrayTexId_;

out vec2 TexCoords;
out float arrayTexID;

uniform vec3 position;

    void main()
    {
        gl_Position = vec4(aPos.x + position.x, aPos.y + position.y, 0.0, 1.0);
        TexCoords = TexCoords_; 
        arrayTexID = arrayTexId_;
    }
