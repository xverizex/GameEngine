#include <Engine/Core/Animation.h>
#include <Engine/Core/Timer.h>
#include <stdlib.h>
#include <stdio.h>

struct anim {
	uint32_t first;
	uint32_t last;
	uint32_t cur_frame;
	uint32_t ms;
	uint32_t once;
	struct times *tm;
};

void anim_delete (struct anim *an)
{
	timer_clear (an->tm);

	delete an;
}

struct anim * 
anim_init (const int size)
{
	struct anim *an = new struct anim[sizeof(struct anim) * size];
	for (int i = 0; i < size; i++) {
		an[i].tm = timer_init (1);
	}
	

	return an;
}

void 
anim_set (struct anim *an, const uint32_t index, const uint32_t first, const uint32_t last, const uint32_t milliseconds)
{
	an[index].first = first;
	an[index].last = last;
	an[index].ms = milliseconds;
	an[index].cur_frame = first;
	an[index].once = 0;
}

uint32_t
anim_run (struct anim *an, const uint32_t index, const uint32_t once)
{
	struct anim *a = &an[index];

	int ret = timer_run (a->tm, 0, a->ms);

	a->cur_frame += ret;
	if (a->cur_frame > a->last) {
		a->cur_frame = a->first;
		if (once == 1) {
			a->once = 1;
			timer_stop (a->tm, 0);
		} else {
			a->once = 0;
		}
	}

	if (a->once == 1) {
		a->cur_frame = a->last;
		timer_stop (a->tm, 0);
		return a->last;
	}

	return a->cur_frame;
}

bool
anim_is_stop (struct anim *an, const uint32_t index)
{
	if (an[index].once == 1) {
		return true;
	}

	return false;
}

void 
anim_stop (struct anim *an, const uint32_t index)
{
	an[index].once = 0;	
}
