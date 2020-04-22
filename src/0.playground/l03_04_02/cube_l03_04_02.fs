#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

// in prev model though we improved diffusion, we were seeing highlight on wood as well not just metal
// hence we need to selectively increase n decrease specular component intensities.
// using same technique that we used for fixing diffusion via texture, we do the same for specular component as well.
// in this case let's say we have texture image where we have black in all the places where we dont want
// specular component n metal in rest of the places.
struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    float shininess; // scattering i,e, radius of specular highlight
}; // since subcomponent is sampler2D- an opaque type, variables of this type should always be uniform
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
    vec3 ambient = lamp.ambient * vec3(texture(material.diffuseMap, TexCoords));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lamp.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lamp.diffuse * (diff * vec3(texture(material.diffuseMap, TexCoords)));

    vec3 camDir = normalize(camPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(camDir, reflectDir), 0.0), material.shininess);
    vec3 specular =  lamp.specular * (spec * vec3(texture(material.specularMap, TexCoords)));

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}