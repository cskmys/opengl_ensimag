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
    // a flashlight is aimed striaght from player's perspective. Hence, it's light-fragment direction needs to be
    // dynamically computed and updated.
    vec3 position;
    vec3 direction;
    float cosCutOff;

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
    vec3 color;
    vec3 lightDir = normalize(lamp.position - FragPos);
    float cosTheta = dot(lightDir, normalize(-lamp.direction));
    // if we were comparing angles then theta < cutOff angle would be inside cone
    // but since we are comparing cosines, it's cosTheta > cutOff
    if(cosTheta > lamp.cosCutOff){
        // inside cone
        vec3 ambient = lamp.ambient * texture(material.diffuseMap, TexCoords).rgb;

        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = lamp.diffuse * (diff * texture(material.diffuseMap, TexCoords).rgb);

        vec3 camDir = normalize(camPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(camDir, reflectDir), 0.0), material.shininess);
        vec3 specular =  lamp.specular * (spec * texture(material.specularMap, TexCoords).rgb);

        // for a point light, we need to know distance from point source so that we can attenaute the light intensity
        float distance    = length(lamp.position - FragPos);
        float attenuation = 1.0 / (lamp.constant + (lamp.linear * distance) + (lamp.quadratic * (distance * distance)));

        // attenuation removed for ambient coz if you dont, at large distances,
        // light of surface would be more than the light inside the flashlight cone even if they are outside the cone
        // ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        color = ambient + diffuse + specular;
    } else { // else, use ambient light so scene isn't completely dark outside the spotlight.
        // if we forget to do this whole world except flash cone, will be in dark
        color = lamp.ambient * texture(material.diffuseMap, TexCoords).rgb;
    }
    FragColor = vec4(color, 1.0);
}