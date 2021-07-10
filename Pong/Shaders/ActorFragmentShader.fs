#version 410 core
    out vec4 FragColor;
in vec3 Normals;


in VS_OUT {
    vec3 fragPos;
    vec2 TexCoords;
    vec3 TangentLightDir;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

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
    vec3 viewDir = vec3(fs_in.TangentViewPos-fs_in.TangentFragPos);
    float fogFactor = exp(-pow(length(viewDir)*0.03,2));
    vec3 norm = normalize(texture(texture1, fs_in.TexCoords).rgb * 2.0 - 1.0);
    
    vec3 fragColor = vec3(0.0);
    
    fragColor += CalcDirLight(dirLight, norm, viewDir);

    FragColor = vec4(fragColor, 1.0);


}

vec3 CalcDirLight(DirLight dirLight, vec3 norm, vec3 viewDir) {
    
    vec3 dirlightDir = normalize(-fs_in.TangentLightDir);
    
    float diff = 0;
    float spec = 0;

    viewDir = normalize(viewDir);
        
    diff = max(dot(norm,dirlightDir),0.0);
        
        vec3 reflectDir = reflect(-dirlightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    
    vec3 ambient = dirLight.ambient*vec3(texture(texture0, fs_in.TexCoords));

    
    vec3 diffuse = dirLight.diffuse*diff*vec3(texture(texture0, fs_in.TexCoords));
    
//    vec3 specular = dirLight.specular * spec * vec3(texture(texture1, TexCoords));

    return (ambient + diffuse);
}
