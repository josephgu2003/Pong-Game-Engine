#version 410 core
    out vec4 FragColor;
    in vec2 TexCoords;

    uniform sampler2D textTexture;

    void main()
    {
        float red = texture(textTexture, TexCoords).r;
        vec2 tex_offset = 1.0 / textureSize(textTexture, 0);
        vec3 color = vec3(1,1,1);
        if(texture(textTexture, TexCoords + vec2(tex_offset.x*2,0)).r < 0.2) {
            color = vec3(0,0,0);
        }
           if(texture(textTexture, TexCoords - vec2(tex_offset.x*2,0)).r < 0.2) {
               color = vec3(0,0,0);
           }
              if(texture(textTexture, TexCoords + vec2(0,tex_offset.y*2)).r < 0.2) {
                  color = vec3(0,0,0);
              }
                 if(texture(textTexture, TexCoords - vec2(0,tex_offset.y*2)).r < 0.2) {
                     color = vec3(0,0,0);
                 }
        FragColor = vec4(color,red);
    }
