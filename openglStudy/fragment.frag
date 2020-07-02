#version 330 core
in vec4 triangleColor;
in vec2 triangleTextureCord;
out vec4 fragmentColor;
uniform sampler2D sampleTexture;
void main(){

	fragmentColor = texture(sampleTexture, triangleTextureCord) * triangleColor;

}