#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aPosRight;

out vec4 PosRight;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	PosRight = projection * view * model * vec4(aPosRight, 1.0f);
}