#version 410 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 TexCoords_;
    
    out vec2 TexCoords;

    void main()
    {
        gl_Position =  vec4(aPos.x , aPos.y , 0, 1.0);
        TexCoords = TexCoords_;
    }
