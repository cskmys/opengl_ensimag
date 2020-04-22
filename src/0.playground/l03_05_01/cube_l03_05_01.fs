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
    // since we are modelling directional light, it's position is not important, coz we assume it to be at infinity.
    // Hence, all the rays are parallel
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Lamp lamp;
uniform vec3 camPos;

void main(){
    vec3 ambient = lamp.ambient * vec3(texture(material.diffuseMap, TexCoords));

    vec3 norm = normalize(Normal);
    // since all rays are parallel, the lighting calculations will remain same regardless of frag position
    // The lighting calculations we used before expect the light direction to be a direction from the fragment
    // towards the light source, but people generally prefer to specify a directional light as a global direction
    // pointing from the light source. Hence, we put a -ve
    vec3 lightDir = normalize(-lamp.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lamp.diffuse * (diff * vec3(texture(material.diffuseMap, TexCoords)));

    vec3 camDir = normalize(camPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(camDir, reflectDir), 0.0), material.shininess);
    vec3 specular =  lamp.specular * (spec * vec3(texture(material.specularMap, TexCoords)));

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}