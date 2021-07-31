#version 410 core
#extension GL_EXT_texture_array : enable

    out vec4 FragColor;

    in vec2 TexCoords;

    uniform sampler2DArray diffuse;
    uniform sampler2D gradient;

in float duration;

    void main()
    {
     //   vec2 tex_offset = 1.0 / textureSize(texture0, 0);
      //  vec2 realCoords = TexCoords-vec2(0,0);
     //   vec4 sampled = texture(texture0, vec3(TexCoords, 13-floor(duration)));
     //   int layer = floor(1.5*duration);
       // vec4 sampled = texture(texture0, vec3(TexCoords,280-floor(92*duration)));
        float duration1 = duration;
        duration1 *= float(36.0);
        int layer = 109;
        layer -= int(floor(duration1));
        vec4 sampled = vec4(0.1,0.1,0.1,1.0-texture(diffuse, vec3(gl_PointCoord, layer)).r);
       // vec4 sampled = vec4(1,1,1,texture(texture0, realCoords).r);
    //    vec4 sampled = texture(texture0, realCoords);
      /**  for(int i = 1; i < 5; ++i)
        {
            sampled.w += 0.1*texture(texture0, TexCoords - vec2(tex_offset.x * 50 *i, 0.0)).r / i;
        }**/
        
        float multiplier = duration-1.5;
        float intensity = texture(gradient, gl_PointCoord).b;
        sampled.w *= intensity*intensity;
        sampled.w *= min((5.0/multiplier/multiplier/multiplier/multiplier),1);
        FragColor = sampled;
    }
