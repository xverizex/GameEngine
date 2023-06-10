#ifndef SHADER_2D_SPRITE_H
#define SHADER_2D_SPRITE_H
#include <Engine/Core/IShader.h>

#include <stdint.h>

class Shader2DSprite: public IShader {
	public:
        ~Shader2DSprite() override;
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

#endif
