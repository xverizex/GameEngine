#version 300 es

precision mediump float;

layout (location = 0) in vec3 vPos;

uniform mat4 position;
uniform mat4 model;
uniform mat4 rotate;
uniform mat4 projection;

void main()
{
	gl_Position = projection * position * rotate * model * vec4(vPos, 1.0);
}
