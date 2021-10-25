#version 410 core
    out vec4 FragColor;
    in vec2 TexCoords;

    uniform sampler2D fbotexture;

uniform sampler2D fbotexture1;
    uniform sampler2D noise;
uniform sampler2D gradient;
uniform bool blur;

layout(std140) uniform StopWatch
{
    float time;
};
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

    void main()
    {
        vec3 sampled;
        if (blur) {
        vec2 tex_offset = 1.0 / textureSize(fbotexture, 0);
        
        float distortion = 1.0+16.0 *
        sin(texture(noise, vec2(TexCoords.x,0.1)).r)
            * (texture(noise, vec2(TexCoords.x/7.0,TexCoords.y/100.0+mod(0.05*time,1))).r-0.3)
                    ;
        
        if (distortion <= 0) {
            distortion = 0;
        } else {
            distortion = distortion * distortion / 8.0;
        }
            
            if (TexCoords.x > 0.55) distortion = 0;
            if (TexCoords.y > 0.65) distortion = 0;
            if (TexCoords.x < 0.45) distortion = 0;
            if (TexCoords.y < 0.2) distortion = 0;
        
            distortion *= texture(gradient, vec2(10.0*(TexCoords.x - 0.55), 2.0*(TexCoords.y-0.2))).r;

        sampled =  texture(fbotexture, TexCoords).rgb * weight[0];
        
        for (int i = 1; i < 5; ++i) {
            sampled += 2.0*texture(fbotexture, TexCoords + vec2(0.0, tex_offset.y * i*30.0*distortion)).rgb * weight[i];
        }
            sampled += distortion * vec3(0.1,0.1,0.1) / 100.0;
        } else {
            sampled = texture(fbotexture, TexCoords).rgb;
        }
        
    
        vec3 bloomColor = texture(fbotexture1, TexCoords).rgb;
        sampled += bloomColor;
        
          float gamma = 2.2;
      //  vec3 result = vec3(1.0) - exp(-sampled * exposure); //camera
        
   //     vec3 result = sampled / (sampled + vec3(1.0)); //reinhard simple
        

       vec3 result = sampled;
        result *= 0.6;
        float a = 2.51;
        float b = 0.03;
        float c = 2.43;
        float d = 0.59;
        float e = 0.14;
        result = (result*(a*result+b))/(result*(c*result+d)+e); //approx aces
        
    //    vec3 result = sampled;
        
        result = pow(result, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
      /**  vec4 sampled = texture(fbotexture, TexCoords);
        for(int i = 1; i < 5; ++i)
        {
            sampled = mix(sampled,texture(fbotexture, TexCoords - vec2(0, 0.01*i)),0.7);
        }
      /**  float intensity =texture(texture1, vec2(realCoords.x/20+0.5,realCoords.y)).b;
        sampled.w += texture(texture0, vec2(0.5, realCoords.y)).r * intensity;**/

    }
