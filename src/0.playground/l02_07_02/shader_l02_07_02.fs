#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main(){
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2); // 0.2: pix = 80% tex1 & 20% tex2
}