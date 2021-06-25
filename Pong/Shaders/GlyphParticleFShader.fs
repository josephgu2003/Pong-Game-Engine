#version 410 core
#extension GL_EXT_texture_array : enable

    out vec4 FragColor;

    in vec2 TexCoords;

    uniform sampler2DArray texture0;
    uniform sampler2D texture1;
    uniform float duration;

    void main()
    {
     //   vec2 tex_offset = 1.0 / textureSize(texture0, 0);
      //  vec2 realCoords = TexCoords-vec2(0,0);
     //   vec4 sampled = texture(texture0, vec3(TexCoords, 13-floor(duration)));
     //   int layer = floor(1.5*duration);
       // vec4 sampled = texture(texture0, vec3(TexCoords,280-floor(92*duration)));
        float duration1 = duration;
        duration1 *= 25;
        int layer = 109;
        layer -= int(floor(duration1));
        vec4 sampled = vec4(0.3,0.3,0.3,0.5*texture(texture0, vec3(TexCoords, layer)).r);
       // vec4 sampled = vec4(1,1,1,texture(texture0, realCoords).r);
    //    vec4 sampled = texture(texture0, realCoords);
      /**  for(int i = 1; i < 5; ++i)
        {
            sampled.w += 0.1*texture(texture0, TexCoords - vec2(tex_offset.x * 50 *i, 0.0)).r / i;
        }**/
        
      //  float multiplier = duration-3.5;
        float intensity = texture(texture1, vec2(TexCoords.x,TexCoords.y)).b;
        sampled.w *= min(intensity*intensity*intensity,1.6);
        FragColor = sampled;
    }
