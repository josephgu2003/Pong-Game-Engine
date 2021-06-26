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

struct DirLight {
vec3 dir;

vec3 ambient;
vec3 diffuse;
vec3 specular;
};

layout (std140) uniform Lights
{
   Light light;

    DirLight dirLight;
    
    vec3 viewPos;
};

    uniform sampler2D texture0;
    uniform sampler2D texture1;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
    vec3 viewDir = vec3(viewPos-fragPos);
    float fogFactor = exp(-pow(length(viewDir)*0.03,2));
    vec3 norm = normalize(Normals);
    
    vec3 fragColor = vec3(0.0);
    
    fragColor += CalcDirLight(dirLight, norm, viewDir);
    
    FragColor = mix(vec4(0.6,0.6,0.6,1), vec4(fragColor,1), fogFactor);
    
  /**  vec3 dir = light.pos - fragPos;
    
    float lengthy = length(dir);
    
    float diff = 0;
    float spec = 0;
    
    vec3 viewDir = vec3(viewPos-fragPos);
    
    float fogFactor = exp(-pow(length(viewDir)*0.13,1.5));
    
    if (lengthy < 50) {
    viewDir = normalize(viewDir);
        
    vec3 lightDir = normalize(dir);
        
    diff = max(dot(norm,lightDir),0.0);
        
        vec3 reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    }
    
    float attenuation = 1.0 / (1.0 + 0.09 * lengthy +
                    0.032 * (lengthy*lengthy));

    vec3 ambient = light.ambient*vec3(texture(texture0, TexCoords));
    
    vec4 glow = (1+brightness)*vec4(1,1,1,1);
    
    vec3 diffuse = light.diffuse*diff*vec3(texture(texture0, TexCoords));
    
    vec3 specular = light.specular * spec * vec3(texture(texture1, TexCoords));
    
//    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    vec4 preFogColor = glow * vec4(ambient + diffuse + specular, 1.0);
    
    FragColor = mix(vec4(0.8,0.8,0.8,1), preFogColor, fogFactor);**/
}

vec3 CalcDirLight(DirLight dirLight, vec3 norm, vec3 viewDir) {
    
    vec3 dirlightDir = normalize(-dirLight.dir);
    
    float diff = 0;
    float spec = 0;

    viewDir = normalize(viewDir);
        
    diff = max(dot(norm,dirlightDir),0.0);
        
        vec3 reflectDir = reflect(-dirlightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    
    vec3 ambient = dirLight.ambient*vec3(texture(texture0, TexCoords));

    
    vec3 diffuse = dirLight.diffuse*diff*vec3(texture(texture0, TexCoords));
    
    vec3 specular = dirLight.specular * spec * vec3(texture(texture1, TexCoords));

    return (ambient + diffuse + specular);
}
