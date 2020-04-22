#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

// prev model is very simplistic coz it doesnt address the fact that different parts of object doesn't
// diffuse/reflect light in the same way.
// So we use same idea like a texture, wherein we use an image to wrap the object and
// use the frag position to retrieve a color
struct Material {
   sampler2D diffuseMap; // ambient colors are indirectly calculated from this
   // if you want you can keep, ambient color separately but that would make ambient color same across the object
   // to have true ambient color just like diffuseMap you can create: sampler2D ambientMap;
    vec3 specular; // sppecularStrength
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
    vec3 specular =  lamp.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}