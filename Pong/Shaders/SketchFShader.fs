#version 410 core
    out vec4 FragColor;
    in vec2 TexCoords;
in vec3 Normals;
in vec3 fragPos;


struct Light {
vec3 pos;

vec3 ambient;
vec3 diffuse;
vec3 specular;
    
};
uniform Light light;

    uniform sampler2D texture0;
uniform vec3 viewPos;

void main()
{
    vec3 norm = normalize(Normals);
    
    vec3 dir = light.pos - fragPos;
    
    float lengthy = length(dir);
    
    float diff = 0;
    float spec = 0;
    
    if (lengthy < 50) {
        
    vec3 lightDir = normalize(dir);
        
    diff = max(dot(norm,lightDir),0.0);
        
        vec3 viewDir = normalize(viewPos-fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    }
    
    float attenuation = 1.0 / (1.0 + 0.35 * lengthy +
                    0.44 * (lengthy*lengthy));

    vec3 ambient = light.ambient*vec3(texture(texture0, TexCoords));
    
  //  vec4 glow = (1+brightness)*vec4(1,1,1,1);
    
    vec3 diffuse = light.diffuse*diff*vec3(texture(texture0, TexCoords));
    
    vec3 specular = light.specular * spec * vec3(0.5,0.5,0.5);
    
//    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    FragColor = vec4(ambient + diffuse, texture(texture0, TexCoords).a);
}
