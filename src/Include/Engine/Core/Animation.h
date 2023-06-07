#ifndef ANIMATION_H
#define ANIMATION_H
#include <Engine/Core/Sprite.h>
#include <stdint.h>

void anim_delete (struct anim *an);
struct anim *anim_init(const int size);
void anim_set(struct anim *an, const uint32_t index, const uint32_t first, const uint32_t last, const uint32_t milliseconds);
uint32_t anim_run(struct anim *an, const uint32_t index, const uint32_t once);
void anim_stop(struct anim *an, const uint32_t index);

#endif
