#version 410 core
    out vec4 FragColor;
    in vec2 TexCoords;

    uniform sampler2D fbotexture;
    uniform sampler2D noise;
uniform bool blur;
layout(std140) uniform StopWatch
{
    float time;
};
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

    void main()
    {
        if (blur) {
        vec2 tex_offset = 1.0 / textureSize(fbotexture, 0);
        
        float distortion = 1.0+16.0 *
        sin(texture(noise, vec2(TexCoords.x,0.1)).r)
        //    * (texture(noise, vec2(TexCoords.x/30.0,TexCoords.y/50.0+(mod(0.3*time,1)))).r-0.5)
            * (texture(noise, vec2(TexCoords.x/7.0,TexCoords.y/100.0+mod(0.05*time,1))).r-0.3)
                    ;
        
        if (distortion <= 0) {
            distortion = 0;
        } else {
            distortion = distortion * distortion / 16.0;
        }
            
            if (TexCoords.x > 0.55) distortion = 0;
            if (TexCoords.y > 0.65) distortion = 0;
            if (TexCoords.x < 0.45) distortion = 0;
            if (TexCoords.y < 0.2) distortion = 0;
      //  vec2 newCoords = vec2(TexCoords.x, TexCoords.y-distortion);
        vec3 sampled =  texture(fbotexture, TexCoords).rgb * weight[0];
        
        for (int i = 1; i < 5; ++i) {
            sampled += 2.0*texture(fbotexture, TexCoords + vec2(0.0, tex_offset.y * i*30.0*distortion)).rgb * weight[i];
          //  sampled += texture(fbotexture, TexCoords - vec2(0.0, tex_offset.y * i*16.0*distortion)).rgb * weight[i];
        }
            sampled += distortion * vec3(0.1,0.1,0.1) / 100.0;
        FragColor = vec4(sampled,1);
        } else {

            FragColor = texture(fbotexture, TexCoords);
        }
        

      /**  vec4 sampled = texture(fbotexture, TexCoords);
        for(int i = 1; i < 5; ++i)
        {
            sampled = mix(sampled,texture(fbotexture, TexCoords - vec2(0, 0.01*i)),0.7);
        }
      /**  float intensity =texture(texture1, vec2(realCoords.x/20+0.5,realCoords.y)).b;
        sampled.w += texture(texture0, vec2(0.5, realCoords.y)).r * intensity;**/

    }
