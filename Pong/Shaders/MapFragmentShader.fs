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
    uniform sampler2D texture1;

uniform samplerCube skyBox;
uniform float time;

vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir);

    void main()
    {

   //     float distortion = sin(10*fragPos.x+30*time)+ 0.3*sin(20*fragPos.x+30*time+0.01);
        float distortion = 0.3*sin(-60*time+10*sqrt(fragPos.x*fragPos.x+fragPos.z*fragPos.z));
        vec3 newNormal = vec3(Normals.x+distortion, Normals.y, Normals.z);
        vec3 norm = normalize(newNormal);
        vec3 fragColor = vec3(0.0);
        
     //   vec3 dir = light.pos - fragPos;
        
      //  float lengthy = length(dir);

                
        vec3 viewDir = vec3(viewPos-fragPos);
        
        float fogFactor = exp(-pow(length(viewDir)*0.13,1.5));
        
        fragColor += CalcDirLight(dirLight, norm, viewDir);
        
       // if (lengthy < 30) {
            
     //   vec3 lightDir = normalize(dir);

       //     viewDir = normalize(viewPos-fragPos);
       //    vec3 reflectDir = reflect(-lightDir, norm);
       //     spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
     //       vec3 reflectDir2 = reflect(-viewDir, norm);
        //    reflection *= texture(skyBox, normalize(reflectDir2)).rgb;
      //  }
        
     //   float ao = texture(texture1, TexCoords).r;
        
      //  vec3 ambient = light.ambient*reflection;
        
     //   vec3 diffuse = light.diffuse*diff*reflection;
        
      //  vec3 specular = light.specular*spec*vec3(2,2,2);
        
      //  diffuse  *= attenuation;
      // specular *= attenuation;
        
      //  fragColor += vec3(ambient + diffuse + specular);
        
        vec4 preFogColor = vec4(fragColor, 1.0);
        
        FragColor = mix(vec4(0.6,0.6,0.6,1), preFogColor, fogFactor);
    }

vec3 CalcDirLight(DirLight dirLight, vec3 norm, vec3 viewDir) {
    
    vec3 dirlightDir = normalize(-dirLight.dir);
    
    float diff = 0;
    float spec = 0;
    vec3 reflection = vec3(0.2,0.2,0.2);
    
    viewDir = normalize(viewDir);
        
    diff = max(dot(norm,dirlightDir),0.0);
        
        vec3 reflectDir = reflect(-dirlightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 reflectDir2 = reflect(-viewDir, norm);
      reflection *= texture(skyBox, normalize(reflectDir2)).rgb;
    //reflection = vec3(0.8,0.8,0.8);
    
    vec3 ambient = dirLight.ambient*reflection;

    vec3 diffuse = dirLight.diffuse*diff*reflection;
    
    vec3 specular = dirLight.specular * spec * reflection;

    return (ambient + diffuse + specular);
    

//  vec3 ambient = light.ambient*reflection;

//   vec3 diffuse = light.diffuse*diff*reflection;

//  vec3 specular = light.specular*spec*vec3(2,2,2);

//  diffuse  *= attenuation;
// specular *= attenuation;
}
