#version 330 core
in vec4 triangleColor;
in vec2 triangleTextureCord;
out vec4 fragmentColor;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float radio;
void main(){
	//0.0 返回第一个texture0 1.0 返回第二个texture1 0.2 代表返回 0.8的texture0 和 0.2的texture1
	//fragmentColor = mix(texture(texture0,triangleTextureCord),texture(texture1,vec2(-triangleTextureCord.x,triangleTextureCord.y)),radio)*triangleColor;
	//fragmentColor = mix(texture(texture0,triangleTextureCord),texture(texture1,triangleTextureCord),1.0);
	//fragmentColor =texture(texture1,triangleTextureCord);

	fragmentColor = texture(texture0,triangleTextureCord);
	//fragmentColor = triangleColor;


}