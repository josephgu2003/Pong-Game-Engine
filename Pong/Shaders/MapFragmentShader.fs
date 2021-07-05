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

    uniform sampler2D mapTexture;

uniform sampler2D noise;

uniform samplerCube skyBox;

layout (std140) uniform StopWatch
{
    float time;
};

vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir);

    void main()

    {

   //     float distortion = sin(10*fragPos.x+30*time)+ 0.3*sin(20*fragPos.x+30*time+0.01);
        float radius = sqrt(fragPos.x*fragPos.x+fragPos.z*fragPos.z);
        vec3 newNormal = Normals;
        float margin = radius - 6.0*mod(time,2);
        float x = (fragPos.x+100.0)/200.0;
        float z = (fragPos.z +100.0)/200.0;
        float degree = texture(noise, vec2(x,z+0.25*time/200.0)).r;
        //float degree2 = texture(noise, vec2(x,z+1.25*time/200.0)).r;
        degree = 2.0 * degree -1.0;
        
        newNormal.z += degree*0.02*sin(3.0*time+12.0*(fragPos.z));
        float degree3 = 0.08*texture(noise, vec2(float(0.002*time+TexCoords.x), TexCoords.y)).r;
        newNormal.z += degree3;
        
        if (abs(margin) < 0.3) {
        float offset = margin/0.3;
        float distortion = degree*0.2*cos(offset*3.14/2.0);
        newNormal = vec3(Normals.x-distortion, Normals.y, Normals.z);
        }
        
        vec3 norm = normalize(newNormal);
        vec3 fragColor = vec3(0.0);
        
        vec3 viewDir = vec3(viewPos-fragPos);
        
        float fogFactor = exp(-pow(length(viewDir)*0.03,2));
        
        fragColor += CalcDirLight(dirLight, norm, viewDir);
        fragColor = mix(fragColor,texture(mapTexture, TexCoords).rgb, texture(mapTexture,TexCoords).a);
        FragColor = vec4(fragColor, fogFactor);
    }

vec3 CalcDirLight(DirLight dirLight, vec3 norm, vec3 viewDir) {
    
    vec3 dirlightDir = normalize(-dirLight.dir);
    
    float diff = 0;
    float spec = 0;
    
    viewDir = normalize(viewDir);
        
    diff = max(dot(norm,dirlightDir),0.0);
        
        vec3 reflectDir = reflect(-dirlightDir, norm);
    spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    
    vec3 reflectDir2 = reflect(-viewDir, norm);

  //  if (reflectDir2.y > 0.0) {
     vec3 reflection = 0.25*texture(skyBox, normalize(reflectDir2)).rgb;
    vec3 direction = vec3(reflectDir2.x,0.0,reflectDir2.z);
   // }
    
    vec3 water = vec3(0.00,0.04,0.2);
    
    vec3 ambient = dirLight.ambient * water * 0.2;

 //   vec3 diffuse = dirLight.diffuse*diff * water * 0.2;
    
    vec3 specular = (dirLight.specular) * spec * 0.5;

    return (ambient+specular+reflection);
    
}
