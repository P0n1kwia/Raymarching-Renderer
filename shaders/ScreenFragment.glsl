#version 460 core

uniform vec2 aResolution;

out vec4 FragColor;
void main()
{
	
	vec2 uv = vec2((gl_FragCoord.xy - 0.5*aResolution)/aResolution.y);
	FragColor = vec4(uv,0.0f,1.0f);
}