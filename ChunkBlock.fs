#version 430 core
out vec4 FragColor;

in vec2 texOut;

uniform sampler2D ourTexture;

void main()
{
	FragColor = texture(ourTexture, texOut);
}