#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 Normals_;
layout (location = 2) in vec2 TexCoords_;

    out vec2 TexCoords;
out vec3 Normals;
out vec3 fragPos;
    
    uniform mat4 modelMat;

layout (std140) uniform ViewProj
{
    mat4 viewMat;

    mat4 projMat;
};

    void main()
    {
        gl_Position =  projMat * viewMat * modelMat * vec4(0.005*aPos.x , 0.005*aPos.y , 0.005*aPos.z, 1.0);
        fragPos = vec3(modelMat*vec4(0.005*aPos, 1.0));
        TexCoords = TexCoords_;
      //  Normals = modelMat* vec4(Normals_,1.0);
        Normals = mat3(transpose(inverse(modelMat))) * Normals_;
    }
