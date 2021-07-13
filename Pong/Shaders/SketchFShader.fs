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

layout (std140) uniform StopWatch
{
    float time;
};

    uniform sampler2D texture0;

uniform float alpha;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
    float alpha_ = texture(texture0, TexCoords).a;
    if(alpha_ < 0.5)
        discard;
    vec3 viewDir = vec3(viewPos-fragPos);
    float fogFactor = exp(-pow(length(viewDir)*0.03,2));
    vec3 norm = normalize(Normals);
    
    vec3 fragColor = vec3(0.0);
    
    fragColor += CalcDirLight(dirLight, norm, viewDir);
    
    //Yirou these next 3 lines can provide fish vfx, my version makes colors flash based on how much red they have, i couldn't figure out anything cooler yet

    fragColor *= 2.0;

    FragColor = mix(vec4(0.6,0.6,0.6,1), vec4(fragColor,alpha_), fogFactor);
    FragColor.a = alpha;
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
    
  //  vec3 specular = dirLight.specular * spec * vec3(texture(texture0, TexCoords));

    return (ambient + diffuse);
}
