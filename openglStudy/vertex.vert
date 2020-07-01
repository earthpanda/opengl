# version 330 core
layout (location = 0) in vec3 aPos;
out vec4 triangleColor;
uniform float translateX;
void main(){
	gl_Position=vec4(aPos.x+translateX,aPos.y,aPos.z,1.0);
	triangleColor=vec4(1.0,0.0,0.0,1.0);
}