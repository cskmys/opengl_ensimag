#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    // light would depend on the orientation & dimensions of object in the world. Hence, to get fragment position,
    // multiply only the model matrix with vertex position
    FragPos = vec3(model * vec4(aPos, 1.0));
    // to correct normal under non-unfiorm scaling of model
    Normal = mat3(transpose(inverse(model))) * aNormal;
    // inverse operations are quite costly on GPU and doing on each vertex is not good approach.
    // so better to calculate the normal matrix on CPU n then send it go GPU
}