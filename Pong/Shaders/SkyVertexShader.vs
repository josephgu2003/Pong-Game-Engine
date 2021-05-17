#version 410 core
    layout (location = 0) in vec3 aPos;
    
    out vec3 TexVec;

    uniform mat4 viewMat;

    uniform mat4 projMat;

    void main()
    {
        TexVec = aPos;
        vec4 pos = projMat * viewMat * vec4(aPos.x , aPos.y, aPos.z, 1.0);
        gl_Position = pos.xyww;
    }
