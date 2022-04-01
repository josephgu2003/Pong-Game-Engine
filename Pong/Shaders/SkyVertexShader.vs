#version 410 core
    layout (location = 0) in vec3 aPos;
    
    out vec3 TexVec;

uniform mat4 viewProjMat2;

    void main()
    {
        TexVec = aPos;
        vec4 pos = viewProjMat2 * vec4(aPos.x , aPos.y, aPos.z, 1.0);
        gl_Position = pos.xyww; 
    }
