#version 410 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 TexCoords_;
    
    out vec2 TexCoords;
    out vec3 fragPos;

    uniform mat4 modelMat;

    uniform mat4 viewMat;

    uniform mat4 projMat;

    void main()
    {
        gl_Position =  projMat * viewMat * modelMat * vec4(30*aPos, 1.0);
        fragPos = vec3(modelMat*vec4(30*aPos, 1.0));
        TexCoords = TexCoords_;
    }
