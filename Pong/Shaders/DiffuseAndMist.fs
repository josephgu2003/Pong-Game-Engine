#version 410 core
layout (location = 0) out vec4 FragColor;

    in vec2 TexCoords;

uniform sampler2D diffuse;

layout (std140) uniform DistanceFog
{
    float fogDensity;
    float fogGradient;
    float frustrumNear;
    float frustrumFar;
    vec3 fogColor;
};

void applyDistanceFog(inout vec3 color);

    void main()

    {
        vec4 color = texture(diffuse, TexCoords);
        
        if (color.a <= 0.4f) {
                discard;
        }
        
        applyDistanceFog(color.rgb);
        FragColor = color;
    }

 
void applyDistanceFog(inout vec3 color) {
    float z = 2.0*gl_FragCoord.z-1.0;
    z = (2.0 * frustrumNear * frustrumFar) / (frustrumFar + frustrumNear - z * (frustrumFar - frustrumNear));
    float fogFactor = exp(-pow(z*fogDensity,fogGradient));
    color = mix(fogColor , color,  fogFactor);
}
