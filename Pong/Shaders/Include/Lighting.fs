// following tutorial: https://learnopengl.com/Lighting/Light-casters

struct Light {
vec3 pos;

vec3 ambient;
vec3 diffuse;
vec3 specular;
    
float constant;
float linear;
float quadratic;
};


struct DirLight {
vec3 dir;

vec3 ambient;
vec3 diffuse;
vec3 specular;
};

vec3 CalcDirLight(DirLight dirLight, vec3 dirlightDir, vec3 norm, vec3 viewDir, vec3 diffusecolor, vec3 specularcolor) {
    float diff = 0;
    float spec = 0;
        
    diff = max(dot(norm,dirlightDir),0.0);
        
    vec3 reflectDir = reflect(-dirlightDir, norm);
    viewDir = normalize(viewDir);
    spec = pow(max(dot(viewDir, reflectDir), 0.0), 30);
    
    vec3 ambient = dirLight.ambient * diffusecolor;

    vec3 diffuse = dirLight.diffuse * diffusecolor * diff;
    
    vec3 specular = dirLight.specular * specularcolor * spec;

    return (ambient+diffuse+specular);
}

vec3 CalcDirLight(DirLight dirLight, vec3 dirlightDir, vec3 norm, vec3 viewDir, vec3 ambientcolor, vec3 diffusecolor, vec3 specularcolor) {
    float diff = 0;
    float spec = 0;
        
    diff = max(dot(norm,dirlightDir),0.0);
        
    vec3 reflectDir = reflect(-dirlightDir, norm);
    viewDir = normalize(viewDir);
    spec = pow(max(dot(viewDir, reflectDir), 0.0), 30);
    
    vec3 ambient = dirLight.ambient * ambientcolor;

    vec3 diffuse = dirLight.diffuse * diffusecolor * diff;
    
    vec3 specular = dirLight.specular * specularcolor * spec;

    return (ambient+diffuse+specular);
}

vec3 CalcPointLight(Light light, vec3 fragPos, vec3 norm, vec3 viewDir, vec3 ambientcolor, vec3 diffusecolor, vec3 specularcolor) {
    float diff = 0;
    float spec = 0;
        
    vec3 lightDir = light.pos - fragPos;
    float dist = length(lightDir);
    lightDir = normalize(lightDir);
    diff = max(dot(norm,lightDir),0.0);
        
    vec3 reflectDir = reflect(-lightDir, norm);
    viewDir = normalize(viewDir);
    spec = pow(max(dot(viewDir, reflectDir), 0.0), 30);
    
    vec3 ambient = light.ambient * ambientcolor;

    vec3 diffuse = light.diffuse * diffusecolor * diff;
    
    vec3 specular = light.specular * specularcolor * spec;
    
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic*dist*dist);

    return (ambient+diffuse+specular) * attenuation;
}
