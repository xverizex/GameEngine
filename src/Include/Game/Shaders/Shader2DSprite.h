#ifndef ENGINE_SHADER_2D_SPRITE_HEADER_H
#define ENGINE_SHADER_2D_SPRITE_HEADER_H
#include <Core/IShader.h>

#include <stdint.h>

namespace Engine {

class Shader2DSprite: public IShader {
	public:
		void init () override;
		void render (void *_data) override;

		uint32_t program;
		uint32_t uniform_model;
		uint32_t uniform_cam;
		uint32_t uniform_projection;
		uint32_t uniform_translation;
		uint32_t uniform_scale;
		uint32_t uniform_sampler;
		uint32_t uniform_rotate;
};

};

#endif
