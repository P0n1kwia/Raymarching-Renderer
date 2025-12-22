
// That is a template for shader fragment.

#version 460 core

uniform vec2 u_mouse;
uniform vec2 u_resolution;
uniform float u_time;

out vec4 FragColor;
void main()
{
	vec2 uv = vec2((gl_FragCoord.xy - 0.5f*u_resolution)/u_resolution.y);
	vec2 m = vec2((u_mouse -0.5f*u_resolution)/u_resolution.y);
	
	FragColor = vec4(uv,sin(u_time),1.0f);
}