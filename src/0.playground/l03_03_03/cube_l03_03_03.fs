#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

// objectColor and few constants from before replaced by this struct
struct Material {
    vec3 ambient; // ambientStrength
    vec3 diffuse; //
    vec3 specular; // sppecularStrength
    float shininess; // scattering i,e, radius of specular highlight
};
uniform Material material;

struct Lamp {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};
uniform Lamp lamp;

uniform vec3 camPos;

void main(){
    vec3 ambient = lamp.ambient * material.ambient;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lamp.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lamp.diffuse * (diff * material.diffuse);

    vec3 camDir = normalize(camPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(camDir, reflectDir), 0.0), material.shininess);
    vec3 specular =  lamp.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}