#version 410 core
#extension GL_EXT_texture_array : enable

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;
    in float arrayTexID; 

    uniform sampler2DArray diffuse;
uniform float brightness;
uniform float alpha = 1.0;

    void main()
    {
        float mipmapLevel = textureQueryLod(diffuse, TexCoords).x;

        int id = int(floor(arrayTexID)); //lmao glsl
        float red = texture(diffuse, vec3(TexCoords, id)).r;
        float a = red;
        vec2 tex_offset = 0.5/ vec2(textureSize(diffuse, int(mipmapLevel)));
        vec3 color = vec3(1.0,1.0,1.0);
        if (red < 0.5) {
         /**   if(texture(diffuse, vec3(TexCoords.x + tex_offset.x*2.0, TexCoords.y, arrayTexID)).r > 0.5) {
                color = vec3(0,0,0);
                alpha = 1.0;
            }
            if(texture(diffuse, vec3(TexCoords.x + tex_offset.x*2.0, TexCoords.y - tex_offset.y*2.0, arrayTexID)).r > 0.5) {
                color = vec3(0,0,0);
                alpha = 1.0;
            }
            if(texture(diffuse, vec3(TexCoords.x + tex_offset.x*2.0, TexCoords.y + tex_offset.y*2.0, arrayTexID)).r > 0.5) {
                color = vec3(0,0,0);
                alpha = 1.0;
            }
          if(texture(diffuse, vec3(TexCoords.x - tex_offset.x*2.0, TexCoords.y, arrayTexID)).r > 0.5) {
                color = vec3(0,0,0);
                alpha = 1.0;
            }
            if(texture(diffuse, vec3(TexCoords.x - tex_offset.x*2.0, TexCoords.y + tex_offset.y*2.0, arrayTexID)).r > 0.5) {
                   color = vec3(0,0,0);
                alpha = 1.0;
            }**/

            if(texture(diffuse, vec3(TexCoords.x - tex_offset.x*4.0, TexCoords.y - tex_offset.y*4.0, arrayTexID)).r > 0.5) {
                   color = vec3(0,0,0);
                a = 1.0;
            }
          /**  if(texture(diffuse, vec3(TexCoords.x, TexCoords.y + tex_offset.y*2.0, arrayTexID)).r > 0.5) {
                      color = vec3(0,0,0);
                alpha = 1.0;
            }
            if(texture(diffuse, vec3(TexCoords.x, TexCoords.y - tex_offset.y*2.0, arrayTexID)).r > 0.5) {
                         color = vec3(0,0,0);
                alpha = 1.0;
            }**/
        }
        FragColor = vec4(color,alpha*a);
        if (brightness > 0.0) {
        BrightColor = brightness * FragColor;
        }
    }
