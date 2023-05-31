#include "Rect.h"
#include "Shaders.h"
#include <cstring>
#include <GLES3/gl3.h>

Rect::Rect(): UI()
{
	width = 0;
	height = 0;

	shader = getIShader(SHADER_RECT);
}

Rect::Rect(uint32_t _width, uint32_t _height)
{
	setSize(_width, _height);
	shader = getIShader(SHADER_RECT);
}

void Rect::resized(uint32_t w, uint32_t h)
{
	
}

void Rect::set_radius (float r)
{
	top_left_radius = r;
	top_right_radius = r;
	bottom_left_radius = r;
	bottom_right_radius = r;
}

const glm::vec4 &Rect::getColor() const
{
	return color;
}

void Rect::setSize(uint32_t _width, uint32_t _height)
{
	width = _width;
	height = _height;

	VertexData *vdata = vertexData;

	float *v = vdata->f[0];

	float w = static_cast<float>(_width);
	float h = static_cast<float>(_height);

	float vd[18] = {
		0.f, 0.f, 0.f,
		0.f, h, 0.f,
		w, 0.f, 0.f,
		w, 0.f, 0.f,
		w, h, 0.f,
		0.f, h, 0.f
	};

	memcpy (v, vd, sizeof(float) * 18);

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 18, v);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Object::resizeMatrix();
}

void Rect::setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	color.r = red / 255.f;
	color.g = green / 255.f;
	color.b = blue / 255.f;
	color.a = alpha / 255.f;
}

void Rect::draw()
{
	if (shader)
		shader->render(this);
}

