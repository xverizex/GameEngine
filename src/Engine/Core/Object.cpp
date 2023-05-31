#include "Object.h"
#include <string.h>
#include "AppConfig.h"
#include "Downloader.h"
#include "Shaders.h"
#include <GLES3/gl3.h>

Object::Object(TYPE_OBJECT type, uint32_t res)
{
	shader = nullptr;

	typeObject = type;
	mmodel = glm::mat4(1.f);
	mrotate = glm::mat4(1.f);
	mposition = glm::mat4(0.f);
	mscale = glm::mat4(1.f);

	switch (type) {
		case TYPE_OBJECT::UI:
			initUI(res);
			break;
	}
}

Object::Object(TYPE_OBJECT type, uint32_t tex, uint32_t width, uint32_t height)
{
	shader = nullptr;

	typeObject = type;
	mmodel = glm::mat4(1.f);
	mrotate = glm::mat4(1.f);
	mposition = glm::mat4(0.f);
	mscale = glm::mat4(1.f);
}

void Object::setPos(glm::vec3 pos)
{
	vpos = pos;
	mposition = glm::translate (mmodel, pos);
}

void Object::rotate(glm::vec3 rot)
{

}

const glm::vec3 &Object::getRotateVector() const
{
	return vrotate;
}

void Object::draw()
{
	
}

uint32_t Object::getTypeObject() const
{
	return typeObject;
}

const glm::mat4 &Object::position() const
{
	return mposition;
}

const glm::mat4 &Object::projection() const
{
	return mprojection;
}

const glm::mat4 &Object::model() const
{
	return mmodel;
}

const glm::mat4 &Object::rotate() const
{
	return mrotate;
}

const glm::mat4 &Object::scale() const
{
	return mscale;
}


const glm::vec3 &Object::getPosVector() const
{
	return vpos;
}

#include <cstdio>

void Object::resizeMatrix()
{
	AppConfig *app = AppConfig::getInstance();
	printf ("%f %f\n", app->app_width_float, app->app_height_float);

	switch(typeObject) {
		default:
			mprojection = glm::ortho(0.f, app->app_width_float, 0.f, app->app_height_float, -0.1f, 10.f);
			break;
	}
}

void Object::initUI(uint32_t res)
{
	AppConfig *app = AppConfig::getInstance();
	float w = static_cast<float>(app->app_width);
	float h = static_cast<float>(app->app_height);

	mprojection = glm::ortho(0.f, w, 0.f, h, -0.1f, 10.f);

	typeShapeRender = GL_TRIANGLES;

	vertexData = new VertexData();

	vertexData->f = new float*[1];
	vertexData->size_f = 1;
	vertexData->max_vertex = 6;
	vertexData->count_v = 18;

	static float vd[18] = {
		0.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		1.f, 0.f, 0.f,
		1.f, 0.f, 0.f,
		1.f, 1.f, 0.f,
		0.f, 1.f, 0.f
	};

	vertexData->f[0] = new float[18];

	memcpy(vertexData->f[0], vd, sizeof(float) * 18);

	vao = new uint32_t[vertexData->size_f];
	vbo = new uint32_t[vertexData->size_f];

	glGenVertexArrays(vertexData->size_f, vao);
	glGenBuffers(vertexData->size_f, vbo);

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexData->count_v, vertexData->f[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Object::tick ()
{
	
}
