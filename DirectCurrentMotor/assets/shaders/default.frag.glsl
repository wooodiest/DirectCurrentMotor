#version 330 core

layout (location = 0) out vec4 color;

in vec3 v_Position;

void main()
{
	color = vec4(v_Position.x + 1.0, v_Position.y + 1.0,v_Position.z + 1.0, 1.0);
}