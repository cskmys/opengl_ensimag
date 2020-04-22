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
uniform Material material;
struct Lamp {
    // prev flashlight cone had hard edges, now we are smoothening it out by defining another metric called outer cone
    // anything within inner will have intensity > 1 & outside < 0 & in between will (0,1)
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
uniform Lamp lamp;
uniform vec3 camPos;

void main(){
    vec3 ambient = lamp.ambient * texture(material.diffuseMap, TexCoords).rgb;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lamp.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lamp.diffuse * (diff * texture(material.diffuseMap, TexCoords).rgb);

    vec3 camDir = normalize(camPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(camDir, reflectDir), 0.0), material.shininess);
    vec3 specular =  lamp.specular * (spec * texture(material.specularMap, TexCoords).rgb);

    float cosInnerTheta = dot(lightDir, normalize(-lamp.direction));
    float epsilon   = lamp.cosInnerCutOff - lamp.cosOuterCutOff;
    // clamp puts the value between the bounds (0.0,1.0)
    float intensity = clamp((cosInnerTheta - lamp.cosOuterCutOff) / epsilon, 0.0, 1.0);
    // intensity removed for ambient coz if you dont, at large distances,
    // light of surface would be more than the light inside the flashlight cone even if they are outside the cone
    // ambient *= intensity;
    diffuse *= intensity;
    specular *= intensity;

    // for a point light, we need to know distance from point source so that we can attenaute the light intensity
    float distance    = length(lamp.position - FragPos);
    float attenuation = 1.0 / (lamp.constant + (lamp.linear * distance) + (lamp.quadratic * (distance * distance)));

    // attenuation removed for ambient coz if you dont, at large distances,
    // light of surface would be more than the light inside the flashlight cone even if they are outside the cone
    // ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}