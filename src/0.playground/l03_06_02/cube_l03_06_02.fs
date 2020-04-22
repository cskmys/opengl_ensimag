#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    float shininess;
};
struct DirLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct FlashLight { // a spotlight that moves ;-)
    vec3 position;
    vec3 direction;
    float cosInnerCutOff;
    float cosOuterCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4

uniform Material material;
uniform DirLight dLamp;
uniform PointLight pLamp[NR_POINT_LIGHTS];
uniform FlashLight fLamp;
uniform vec3 camPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 camDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 camDir);
vec3 CalcFlashLight(FlashLight light, vec3 normal, vec3 fragPos, vec3 camDir);

void main(){
    vec3 norm = normalize(Normal);
    vec3 camDir = normalize(camPos - FragPos);

    vec3 result = CalcDirLight(dLamp, norm, camDir);
    for(int i = 0; i < NR_POINT_LIGHTS; i++){
        result += CalcPointLight(pLamp[i], norm, FragPos, camDir);
    }

    result += CalcFlashLight(fLamp, norm, FragPos, camDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight lamp, vec3 normal, vec3 camDir){
    vec3 ambient  = lamp.ambient  * texture(material.diffuseMap, TexCoords).rgb;

    vec3 lightDir = normalize(-lamp.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse  = lamp.diffuse  * diff * texture(material.diffuseMap, TexCoords).rgb;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(camDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lamp.specular * spec * texture(material.specularMap, TexCoords).rgb;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight lamp, vec3 normal, vec3 fragPos, vec3 camDir){
    vec3 ambient  = lamp.ambient * texture(material.diffuseMap, TexCoords).rgb;

    vec3 lightDir = normalize(lamp.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = lamp.diffuse * (diff * texture(material.diffuseMap, TexCoords).rgb);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(camDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lamp.specular * (spec * texture(material.specularMap, TexCoords).rgb);

    float distance = length(lamp.position - fragPos);
    float attenuation = 1.0 / (lamp.constant + (lamp.linear * distance) + (lamp.quadratic * (distance * distance)));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcFlashLight(FlashLight lamp, vec3 normal, vec3 fragPos, vec3 camDir){
    vec3 ambient = lamp.ambient * texture(material.diffuseMap, TexCoords).rgb;

    vec3 lightDir = normalize(lamp.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = lamp.diffuse * (diff * texture(material.diffuseMap, TexCoords).rgb);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(camDir, reflectDir), 0.0), material.shininess);
    vec3 specular =  lamp.specular * (spec * texture(material.specularMap, TexCoords).rgb);

    float cosInnerTheta = dot(lightDir, normalize(-lamp.direction));
    float epsilon   = lamp.cosInnerCutOff - lamp.cosOuterCutOff;

    float intensity = clamp((cosInnerTheta - lamp.cosOuterCutOff) / epsilon, 0.0, 1.0);

    diffuse *= intensity;
    specular *= intensity;

    float distance    = length(lamp.position - fragPos);
    float attenuation = 1.0 / (lamp.constant + (lamp.linear * distance) + (lamp.quadratic * (distance * distance)));

    diffuse *= attenuation;
    specular *= attenuation;

    return(ambient + diffuse + specular);
}