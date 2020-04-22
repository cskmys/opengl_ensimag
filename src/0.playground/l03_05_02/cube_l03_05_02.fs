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
    // for a point light source, position is important. So, our previous implementatins used before directional
    // light was somewhat a point light. a realistic point light's light intensity should reduce as we go further from
    // the light source. to account for that we are adding 3 factos: const, linear, quad
    vec3 position;
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

    // for a point light, we need to know distance from point source so that we can attenaute the light intensity
    float distance    = length(lamp.position - FragPos);
    float attenuation = 1.0 / (lamp.constant + (lamp.linear * distance) + (lamp.quadratic * (distance * distance)));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}