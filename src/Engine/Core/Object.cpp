#include <Engine/Core/Object.h>
#include <string.h>
#include <Engine/Core/Downloader.h>
#include <Engine/Core/ShaderManager.h>
#include <Engine/Core/ScreenManager.h>
#include <Game/Core/ShaderList.h>
#include <asgl.h>
#include <Engine/Core/Animation.h>

Object::Object(TYPE_OBJECT type, uint32_t res, uint32_t count_div)
{
	shader = nullptr;

	typeObject = type;
	mmodel = glm::mat4(1.f);
	mrotate = glm::mat4(1.f);
	mposition = glm::mat4(0.f);
	mscale = glm::mat4(1.f);

	this->count_div = count_div;

	switch (type) {
		case TYPE_OBJECT::UI:
			initUI(res);
			break;
		case TYPE_OBJECT::SPRITE:
			initSprite(res, count_div);
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

	switch (type) {
		case FOREIGN_TEXTURE:
			initForeignTexture(tex, width, height);
			break;
	}
}

Object::~Object ()
{
	if (animation)
		anim_delete (animation);

	uint32_t count = vertexData->size_f;

	glDeleteBuffers (count, vbo);
	glDeleteVertexArrays (count, vao);

	delete[] vbo;
	delete[] vao;

	if (typeObject == FOREIGN_TEXTURE) {
		delete vertexData;
	}
}

void Object::initForeignTexture(uint32_t res, uint32_t width, uint32_t height)
{
	vertexData = new VertexData();

	float ww = static_cast<float>(width);
	float hh = static_cast<float>(height);

	float v[30] = {
		0.f, 0.f, 0.0f, 0.0f, 0.0f,
		0.f, hh, 0.0f, 0.0f, 1.0f,
		ww, 0.f, 0.0f, 1.0f, 0.0f,
		ww, 0.f, 0.0f, 1.0f, 0.0f,
		ww, hh, 0.0f, 1.0f, 1.0f,
		0.f, hh, 0.0f, 0.0f, 1.0f
	};

	vertexData->f = new float*[1];
	vertexData->size_f = 1;
	vertexData->tex_sampler = new uint32_t[1];
	vertexData->tex_sampler[0] = res;
	vertexData->tex_count = 1;
	vertexData->tex_width = width;
	vertexData->tex_height = height;
	
	vertexData->f[0] = new float[30];
	
	memcpy (vertexData->f[0], v, sizeof(float) * 30);

	ScreenManager* screen_manager = ScreenManager::get_instance ();
	float w = screen_manager->width_float;
	float h = screen_manager->height_float;

	mprojection = glm::ortho (0.f, w, 0.f, h, -1.0f, 10.f);
	
	ShaderManager* shader_manager = ShaderManager::get_instance ();
	shader = shader_manager->get_shader(SHADER_SPRITE);

	uint32_t count = vertexData->size_f;

	vao = new uint32_t[count];
	vbo = new uint32_t[count];

	glGenBuffers (count, vbo);
	glGenVertexArrays (count, vao);
	
	for (int i = 0; i < count; i++) {
		
		glBindVertexArray (vao[i]);
		glBindBuffer (GL_ARRAY_BUFFER, vbo[i]);
		glBufferData (GL_ARRAY_BUFFER, sizeof (float) * 30, vertexData->f[i], GL_STATIC_DRAW);

		glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof (float) * 5, (void *) 0);
		glEnableVertexAttribArray (0);
		glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, sizeof (float) * 5, (void *) (sizeof(float) * 3));
		glEnableVertexAttribArray (1);

		glBindBuffer (GL_ARRAY_BUFFER, 0);
		glBindVertexArray (0);

	}
	
	max_vertex = 6;
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


void Object::resizeMatrix()
{
	ScreenManager* screen_manager = ScreenManager::get_instance ();
	float w = screen_manager->width_float;
	float h = screen_manager->height_float;

	switch(typeObject) {
		case UI:
		case SPRITE:
		case FOREIGN_TEXTURE:
			mprojection = glm::ortho(0.f, w, 0.f, h, -0.1f, 10.f);
			break;
	}
}

void Object::initUI(uint32_t res)
{
	ScreenManager* screen_manager = ScreenManager::get_instance ();
	float w = screen_manager->width_float;
	float h = screen_manager->height_float;

	mprojection = glm::ortho(0.f, w, 0.f, h, -0.1f, 10.f);

	typeShapeRender = GL_TRIANGLES;

	vertexData = new VertexData();

	vertexData->f = new float*[1];
	vertexData->size_f = 1;
	vertexData->max_vertex = 6;
	vertexData->count_v = 18;

	float vd[18] = {
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

void Object::set_width (float www)
{
	ScreenManager* screen_manager = ScreenManager::get_instance ();
	float aspect = screen_manager->width_float / screen_manager->height_float;

	uint32_t width = vertexData->tex_width;
	uint32_t height = vertexData->tex_height;

	float resu = 0.f;
	if (width > height) {
		float asp = static_cast<float>(width) / static_cast<float>(height);
		this->aspect = asp;
		if (screen_manager->width_float > screen_manager->height_float) {
			resu = www / asp;
		}
	} else if (height > width) {
		float asp = static_cast<float>(height) / static_cast<float>(width);
		this->aspect = asp;
		if (screen_manager->width_float > screen_manager->height_float) {
			resu = www * asp;
		}
	} else {
		resu = www;
	}

	float ww = www;
	float hh = resu;

	float v[30] = {
		0.f, 0.f, 0.0f, 0.0f, 0.0f,
		0.f, hh, 0.0f, 0.0f, 1.0f,
		ww, 0.f, 0.0f, 1.0f, 0.0f,
		ww, 0.f, 0.0f, 1.0f, 0.0f,
		ww, hh, 0.0f, 1.0f, 1.0f,
		0.f, hh, 0.0f, 0.0f, 1.0f
	};

	w = ww;
	h = hh;

	for (uint32_t i = 0; i < vertexData->size_f; i++) {
		memcpy(vertexData->f[i], v, sizeof(float) * 30);

		glBindVertexArray (vao[i]);
		glBindBuffer (GL_ARRAY_BUFFER, vbo[i]);
		glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof (float) * 30, vertexData->f[i]);
	}
}

void Object::initSprite(uint32_t res, uint32_t count_div)
{
	vertexData = downloader_load_sprite(res);

	ScreenManager* screen_manager = ScreenManager::get_instance ();
	float w = screen_manager->width_float;
	float h = screen_manager->height_float;

	float aspect = screen_manager->width_float / screen_manager->height_float;

	float num = 1.f;
	float size = 14.f;

	if (count_div == 0)
		mprojection = glm::ortho (0.f, screen_manager->width_float, 0.f, screen_manager->height_float, -1.0f, 10.f);
	else
		mprojection = glm::ortho (0.f, static_cast<float>(count_div) * aspect, static_cast<float>(count_div), 0.0f, 0.f, 10.f);
	
	ShaderManager* shader_manager = ShaderManager::get_instance();
	shader = shader_manager->get_shader(SHADER_SPRITE);

	uint32_t count = vertexData->size_f;

	vao = new uint32_t[count];
	vbo = new uint32_t[count];

	glGenBuffers (count, vbo);
	glGenVertexArrays (count, vao);
	
	for (int i = 0; i < count; i++) {
		
		glBindVertexArray (vao[i]);
		glBindBuffer (GL_ARRAY_BUFFER, vbo[i]);
		glBufferData (GL_ARRAY_BUFFER, sizeof (float) * 30, vertexData->f[i], GL_STATIC_DRAW);

		glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof (float) * 5, (void *) 0);
		glEnableVertexAttribArray (0);
		glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, sizeof (float) * 5, (void *) (sizeof(float) * 3));
		glEnableVertexAttribArray (1);

		glBindBuffer (GL_ARRAY_BUFFER, 0);
		glBindVertexArray (0);

	}
	
	max_vertex = 6;
}

void Object::initModel(uint32_t res)
{

}

void Object::tick ()
{
	
}
