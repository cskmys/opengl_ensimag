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
uniform vec3 lampPos;
uniform vec3 camPos;
uniform vec3 lightColor;
// now everything will look too bright, since we are effectively ignoring the ambient, diffuse, and
// specular properties of light & considering them to be lightColor = (1,1,1).
// Instead if we fix those components at a lower value, we will be able solve the problem
void main(){
    vec3 ambient = material.ambient * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lampPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (material.diffuse * diff) * lightColor;

    vec3 camDir = normalize(camPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(camDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (material.specular * spec) * lightColor;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}