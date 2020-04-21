#version 330 core
in vec3 Normal;
// fragPos provided to each vertex, now frag shader interpolates the values in between and performs coloring on all.
in vec3 FragPos;

out vec4 FragColor;

// lamp is stationary, hence better to declare it as uniform and set it just once
uniform vec3 lampPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main(){
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // we just need unit vector perpendicular to surface
    // note: when model is non-unformly scaled, then normals will no longer be perpendicular to surface,
    // you can't just normalize it and take it. Additional math will be required to correct the normal.
    // here whatever we are doing works only for uniform scaling
    vec3 norm = normalize(Normal);
    // light direction is the unit vector between light position and fragment postion
    vec3 lightDir = normalize(lampPos - FragPos);
    // if angle b/w normal n light > 90, then dot prod < 0 which will result in -ve color(which is undefined).
    // Hence, for < 0 it should be = zero.
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}