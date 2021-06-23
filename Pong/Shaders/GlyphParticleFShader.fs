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
        vec2 realCoords = TexCoords-vec2(0,0);
     //   vec4 sampled = texture(texture0, vec3(TexCoords, 13-floor(duration)));
     //   int layer = floor(1.5*duration);
       // vec4 sampled = texture(texture0, vec3(TexCoords,280-floor(92*duration)));
        vec4 sampled = vec4(0.3,0.3,0.3,texture(texture0, vec3(TexCoords, 109.0-floor(20.0*duration))).r);
       // vec4 sampled = vec4(1,1,1,texture(texture0, realCoords).r);
    //    vec4 sampled = texture(texture0, realCoords);
      /**  for(int i = 1; i < 5; ++i)
        {
            sampled.w += 0.1*texture(texture0, TexCoords - vec2(tex_offset.x * 50 *i, 0.0)).r / i;
        }**/
        if (sampled.a < 0.3) sampled.a = 0;
        
        float multiplier = duration-0.75;
        float intensity =texture(texture1, vec2(realCoords.x/20+0.5,realCoords.y)).b;
        sampled.w *= intensity/multiplier/multiplier;
        FragColor = sampled;
    }
