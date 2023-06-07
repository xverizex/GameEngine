#ifndef SPRITE_H
#define SPRITE_H
#include <Engine/Core/Object.h>
#include <cstdint>

class Shader2DSprite;

class Sprite: public Object {
	public:
		Sprite(uint32_t res, uint32_t count_div);
		Sprite(uint32_t tex, uint32_t width, uint32_t height);
		void draw() override;
		void resizeMatrix () override;

		uint32_t cur_tex;
};


#endif
