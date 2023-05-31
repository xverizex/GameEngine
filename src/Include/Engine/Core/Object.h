#ifndef ENGINE_OBJECT_HEADER_H
#define ENGINE_OBJECT_HEADER_H
#include <stdint.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Engine/Core/IShader.h>
#include <Engine/Core/VertexData.h>

enum TYPE_OBJECT {
	UI,
	N_TYPE_OBJECT
};


class Object {
	protected:

	private:
		glm::quat qrotate;
		glm::vec3 vrotate;
		void initUI(uint32_t res);
	protected:
		uint32_t typeObject;
		glm::mat4 mposition;
		glm::mat4 mprojection;
		glm::mat4 mmodel;
		glm::mat4 mrotate;
		glm::mat4 mscale;
		IShader *shader;

		uint32_t *vbo;
		uint32_t typeShapeRender;
		uint32_t max_vertex;
	public:
		glm::vec3 vpos;
		uint32_t *vao;
		VertexData *vertexData;
		uint32_t getTypeObject() const;
		const glm::mat4 &position() const;
		const glm::mat4 &projection() const;
		const glm::mat4 &model() const;
		const glm::mat4 &rotate() const;
		const glm::mat4 &scale() const;
		const glm::vec3 &getPosVector() const;

		Object(TYPE_OBJECT type, uint32_t res);
		Object(TYPE_OBJECT type, uint32_t tex, uint32_t width, uint32_t height);
		virtual void setPos(glm::vec3 pos);
		virtual void rotate(glm::vec3 rot);
		virtual void resizeMatrix();
		const glm::vec3 &getPos();
		const glm::vec3 &getRotateVector() const;
		virtual void tick();
		virtual void draw();
};

#endif
