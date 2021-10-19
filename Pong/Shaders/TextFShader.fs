#version 410 core
#extension GL_EXT_texture_array : enable

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;
    in float arrayTexID; 

    uniform sampler2DArray textTexture;

    void main()
    {
        float mipmapLevel = textureQueryLod(textTexture, TexCoords).x;

        int id = int(floor(arrayTexID)); //lmao glsl
        float red = texture(textTexture, vec3(TexCoords, id)).r;
        float alpha = red;
        vec2 tex_offset = 0.5/ vec2(textureSize(textTexture, int(mipmapLevel)));
        vec3 color = vec3(1.0,1.0,1.0);
      /**  if (red < 0.5) {
            if(texture(textTexture, vec3(TexCoords.x + tex_offset.x*1.0, TexCoords.y, arrayTexID)).r > 0.5) {
                color = vec3(0,0,0);
                alpha = 1.0;
            }
            if(texture(textTexture, vec3(TexCoords.x + tex_offset.x*2.0, TexCoords.y - tex_offset.y*2.0, arrayTexID)).r > 0.5) {
                color = vec3(0,0,0);
                alpha = 1.0;
            } 
            if(texture(textTexture, vec3(TexCoords.x + tex_offset.x*2.0, TexCoords.y + tex_offset.y*2.0, arrayTexID)).r > 0.5) {
                color = vec3(0,0,0);
                alpha = 1.0;
            }
            if(texture(textTexture, vec3(TexCoords.x - tex_offset.x*2.0, TexCoords.y, arrayTexID)).r > 0.5) {
                color = vec3(0,0,0);
                alpha = 1.0;
            }
            if(texture(textTexture, vec3(TexCoords.x - tex_offset.x*2.0, TexCoords.y + tex_offset.y*2.0, arrayTexID)).r > 0.5) {
                   color = vec3(0,0,0);
                alpha = 1.0;
            }

            if(texture(textTexture, vec3(TexCoords.x - tex_offset.x*2.0, TexCoords.y - tex_offset.y*2.0, arrayTexID)).r > 0.5) {
                   color = vec3(0,0,0);
                alpha = 1.0;
            }
            if(texture(textTexture, vec3(TexCoords.x, TexCoords.y + tex_offset.y*2.0, arrayTexID)).r > 0.5) {
                      color = vec3(0,0,0);
                alpha = 1.0;
            }
            if(texture(textTexture, vec3(TexCoords.x, TexCoords.y - tex_offset.y*2.0, arrayTexID)).r > 0.5) {
                         color = vec3(0,0,0);
                alpha = 1.0;
            }
            
        }**/
        FragColor = vec4(color,alpha);
    }
