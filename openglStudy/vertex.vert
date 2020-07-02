# version 330 core
layout ( location = 0 ) in vec3 aPos;
layout ( location = 1 ) in vec3 aColor;
layout ( location = 2 ) in vec2 aTextureCord;
out vec4 triangleColor;
out vec2 triangleTextureCord;
uniform float translateX;
void main(){
	gl_Position=vec4(aPos.x+translateX,aPos.y,aPos.z,1.0);
	triangleColor=vec4(aColor,1.0);
	triangleTextureCord=aTextureCord;
}