#version 430 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aPosRight;

out vec4 fbr;
out vec4 ftl;
out vec4 ftr;

out vec4 rbl;
out vec4 rbr;
out vec4 rtl;
out vec4 rtr;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	fbr = projection * view * model * vec4(vec3(1.0f, 0.0f, 0.0f), 1.0f);
	ftl = projection * view * model * vec4(vec3(0.0f, 1.0f, 0.0f), 1.0f);
	ftr = projection * view * model * vec4(vec3(1.0f, 1.0f, 0.0f), 1.0f);
	
	rbl = projection * view * model * vec4(vec3(0.0f, 0.0f, -1.0f), 1.0f);
	rbr = projection * view * model * vec4(vec3(1.0f, 0.0f, -1.0f), 1.0f);
	rtl = projection * view * model * vec4(vec3(0.0f, 1.0f, -1.0f), 1.0f);
	rtr = projection * view * model * vec4(vec3(1.0f, 1.0f, -1.0f), 1.0f);
}