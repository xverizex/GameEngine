#ifndef ENGINE_SHADER_RECT_HEADER_H
#define ENGINE_SHADER_RECT_HEADER_H
#include <stdint.h>
#include <Engine/Core/IShader.h>
#include <Engine/UI/Rect.h>

class ShaderRect: public IShader {
	public:
        ~ShaderRect() override;
		void init () override;
		void render (void *_data) override;

	protected:
		uint32_t uniform_model;
		uint32_t uniform_color;
		uint32_t uniform_projection;
		uint32_t uniform_rotate;
		uint32_t uniform_position;
		uint32_t uniform_top_left_radius;
		uint32_t uniform_top_right_radius;
		uint32_t uniform_bottom_left_radius;
		uint32_t uniform_bottom_right_radius;
		uint32_t uniform_pos;
		uint32_t uniform_size_quad;

		uint32_t program;
};

#endif
