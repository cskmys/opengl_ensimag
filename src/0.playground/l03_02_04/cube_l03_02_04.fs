#version 330 core
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lampPos;
uniform vec3 camPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main(){
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lampPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5; // to restrict it to medium bright color
    vec3 camDir = normalize(camPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(camDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}