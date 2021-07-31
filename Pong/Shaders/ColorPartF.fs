#version 410 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

    in vec2 TexCoords;

in float duration;

uniform sampler2D gradient;
uniform vec4 color;

  /**  void main()
    {
        if (duration < 0.0) discard;
             vec4 fragColor = vec4(5,5,5,0);
            fragColor.a += texture(gradient, gl_PointCoord).r * sqrt(sqrt(duration / 1.0));
            fragColor.rgb = mix(fragColor.rgb, color.rgb, pow(duration,0.25));
             FragColor = fragColor;
             float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
        if(brightness > 0.0)
            BrightColor = 0.5*color;
        else
           BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }**/

/**oid main()
{
    if (duration < 0.0) discard;
    vec4 fragColor = color;
   fragColor.a = sqrt(texture(gradient, gl_PointCoord).r) * (duration/ 1.0);
   fragColor.rgb = 5.0*mix(fragColor.rgb, vec3(1.0,1.0,1.0), fragColor.a);

    float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
       if(brightness > 0.0)
           BrightColor = vec4(fragColor.rgb, 1.0);
       else
           BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    
    FragColor = 20.0*fragColor;
}**/

/**void main()
{
    if (duration < 0.0) discard;
    vec4 fragColor = vec4(0,0,0,1);
   fragColor.a = sqrt(texture(gradient, gl_PointCoord).r);
  //  fragColor.rgb += pow(fragColor.a,2) * vec3(10.0,10.0,10.0);
    fragColor.rgb += vec3(50.0,50.0,50.0) * pow((duration),0.2);
    fragColor.rgb *= pow((duration),1.5);
    FragColor = fragColor;
    
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    
      if(brightness > 0.0)
          BrightColor = 3.0*pow((duration),0.2)*color;
   else
          BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}**/

void main()
{
    if (duration < 0.0) discard; 
            vec4 fragColor = vec4(0,0,0,1);
           fragColor.a = pow(texture(gradient, gl_PointCoord).r, (3.0-duration)/3.0) * pow(duration, 0.25);
           fragColor.rgb += pow(duration,1.8)*vec3(8.0,8.0,8.0) * fragColor.a;
            FragColor = fragColor;
            float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
               if(brightness > 0.0)
                   BrightColor = 0.2*vec4(color.rgb, 1.0);
               else
                  BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
} 

 
