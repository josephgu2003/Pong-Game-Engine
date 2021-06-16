#version 410 core
    out vec4 FragColor;
    in vec2 TexCoords;

    uniform sampler2D texture0;
    uniform sampler2D texture1;
    uniform float duration;

    void main()
    {
     //   vec2 tex_offset = 1.0 / textureSize(texture0, 0);
        vec2 realCoords = TexCoords-vec2(0,0);
        vec4 sampled = vec4(0.2,0.2,0.2,texture(texture0, TexCoords).r);
       // vec4 sampled = vec4(1,1,1,texture(texture0, realCoords).r);
    //    vec4 sampled = texture(texture0, realCoords);
      /**  for(int i = 1; i < 5; ++i)
        {
            sampled.w += 0.1*texture(texture0, TexCoords - vec2(tex_offset.x * 50 *i, 0.0)).r / i;
        }**/
        float multiplier = duration-2;
      /**  float intensity =texture(texture1, vec2(realCoords.x/20+0.5,realCoords.y)).b;
        sampled.w += texture(texture0, vec2(0.5, realCoords.y)).r * intensity;**/
        sampled.w *= 1/multiplier/multiplier;
        FragColor = sampled;
    }
