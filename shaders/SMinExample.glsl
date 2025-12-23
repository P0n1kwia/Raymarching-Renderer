#version 460 core

uniform vec2 u_mouse;
uniform vec2 u_resolution;
uniform float u_time;

out vec4 FragColor;
#define MAX_ITERATIONS 100

//---------------------------------SDFs---------------------------------
mat2 rotation(float angle)
{
	float s = sin(angle);
	float c = cos(angle);

	return mat2(c,-s,s,c);
}

float sphereSDF(vec3 p, float radius)
{
	return length(p) - radius;
}
float torusSDF(vec3 p, float radius, float thickness)
{
	vec2 q = vec2(length(p.xz) - radius,p.y);

	return length(q) - thickness;

}
float boxSDF(vec3 p, vec3 ab)
{
	vec3 q = abs(p) - ab;
	return length(max(q,0.0f)) + min(max(q.x,max(q.y,q.z)),0.0);
}
float smin( float a, float b, float k )
{
    k *= 6.0;
    float h = max( k-abs(a-b), 0.0 )/k;
    return min(a,b) - h*h*h*k*(1.0/6.0);
}

float scene(vec3 p, vec2 m)
{
	//torus
	vec3 torusRotation = p;
	vec3 toursTranslate = vec3(0,-m.x,0.0);
	float torus = torusSDF(torusRotation - toursTranslate,.6f,0.07f);

	vec3 rectangleSize = vec3(0.5f,0.5f,0.3);
	vec3 rectangleTranslate = vec3(m.x,0.0f,0.0);
	float rectangle = boxSDF(p-rectangleTranslate,rectangleSize);
	
	vec3 sphereTranslation = vec3(-m.x,0,0);
	float sphere = sphereSDF(p-sphereTranslation,0.5f);


	return smin(rectangle,smin(torus,sphere,0.1f),0.1f);
}

void main()
{
	vec2 uv = vec2((gl_FragCoord.xy - 0.5f*u_resolution)/u_resolution.y);
	vec2 m = vec2((u_mouse -0.5f*u_resolution)/u_resolution.y);
	
	vec3 origin = vec3(0,0,-3);
	vec3 dir = normalize(vec3(uv,1));
	float t = 0.f;//distance traveled

	vec3 finalColor = vec3(0.0f);

	for(int i=0;i<MAX_ITERATIONS;++i)
	{
		
		vec3 p = origin + t*dir;

		float d = scene(p,m);

		if(d<=0.001 || t >= 100) break;
		t += d;

	}
	finalColor = vec3(t*0.2);
	FragColor = vec4(finalColor,1.0f);
}