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
    Normal = aNormal; // normal directly passed frag shader
    // shouldn't normals be multiplied with model matrix as well?
    // 1: normals are just directions, they dont have magnitude/specific point in space
    // 2: normals don't have homogenous component i,e, 4th dimension w.
    // Therefore, they are not affected by transformations. Mathematicaly, they can only be multiplied with
    // 3d matrix of model i,e, model without any transformations.
}