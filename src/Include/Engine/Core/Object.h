#ifndef OBJECT_H_H
#define OBJECT_H_H
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Engine/Core/IShader.h>
#include <Engine/Core/VertexData.h>
#include <asgl.h>

enum TYPE_OBJECT {
	UI,
	SPRITE,
	MODEL,
	FOREIGN_TEXTURE,
	N_TYPE_OBJECT
};

struct anim;

class Object {
	protected:

	private:
        float set_widthed = {0.f};
		glm::quat qrotate;
		glm::vec3 vrotate;
		void initUI(uint32_t res);
		void initSprite(uint32_t res, uint32_t count_div);
		void initModel(uint32_t res);
		void initForeignTexture(uint32_t res, uint32_t width, uint32_t height);
        void internal_load_sprite ();
        void internal_unload_sprite ();
	public:
        void load_assets ();
        void unload_assets ();
		float aspect;
		uint32_t typeObject;
		glm::mat4 mposition;
		glm::mat4 mprojection;
		glm::mat4 mmodel;
		glm::mat4 mrotate;
		glm::mat4 mscale;
		IShader *shader;
        uint32_t shader_type;
		float w;
		float h;

        uint32_t type_res = {0};
		uint32_t *vbo = {nullptr};
		uint32_t typeShapeRender;
		glm::vec3 vpos;
		uint32_t max_vertex;
	public:
		uint32_t *vao = {nullptr};
		VertexData *vertexData = {nullptr};
		uint32_t getTypeObject() const;
		const glm::mat4 &position() const;
		const glm::mat4 &projection() const;
		const glm::mat4 &model() const;
		const glm::mat4 &rotate() const;
		const glm::mat4 &scale() const;
		const glm::vec3 &getPosVector() const;

		~Object ();
		Object(TYPE_OBJECT type, uint32_t res, uint32_t count_div);
		Object(TYPE_OBJECT type, uint32_t tex, uint32_t width, uint32_t height);
		virtual void setPos(glm::vec3 pos);
		virtual void rotate(glm::vec3 rot);
		virtual void resizeMatrix();
		virtual void set_width (float w);
		const glm::vec3 &getPos();
		const glm::vec3 &getRotateVector() const;
		virtual void tick();
		virtual void draw();

		uint32_t count_div;

		struct anim* animation = {nullptr};
};

#endif
