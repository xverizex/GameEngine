#version 330

precision mediump float;

uniform vec4 s_color;

layout (location = 0) out vec4 color;

void main ()
{
	color = s_color;
}
