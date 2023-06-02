#include <Engine/Core/Sprite.h>

Sprite::Sprite(uint32_t res): Object(SPRITE, res)
{
	cur_tex = 0;
}

Sprite::Sprite(uint32_t res, uint32_t width, uint32_t height): Object(FOREIGN_TEXTURE, res, width, height)
{
	cur_tex = 0;
}

void Sprite::draw()
{
	shader->render(this);	
}

void Sprite::resizeMatrix()
{
	Object::resizeMatrix();
}
