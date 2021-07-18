#version 410 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

    in vec2 TexCoords;

in float duration;

uniform sampler2D texture0;
uniform vec4 color;

    void main()
    {
        if (duration < 0.0) discard;
        vec4 fragColor = color;
       fragColor.a = sqrt(texture(texture0, gl_PointCoord).r) * sqrt(sqrt(duration / 1.0));
       fragColor.rgb = mix(fragColor.rgb, vec3(1.0,1.0,1.0), fragColor.a);
        FragColor = fragColor;
        float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
           if(brightness > 0.0)
               BrightColor = vec4(FragColor.rgb, 1.0);
           else
              BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
