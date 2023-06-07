#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <chrono>
#include <Engine/Core/Timer.h>

#if WIN32
struct timeval {
	long tv_sec;
	long tv_usec;
};
#endif

struct times *
timer_init (const uint32_t size)
{
	struct times *t = new struct times [sizeof (struct times) * size];
	for (int i = 0; i < size; i++) {
		t[i].cur = 0L;
		t[i].turn = 0L;
	}
	return t;
}

int gettimeofday(struct timeval* tp, struct timezone* tzp) {

	namespace sc = std::chrono;
	sc::system_clock::duration d = sc::system_clock::now().time_since_epoch();
	sc::seconds s = sc::duration_cast<sc::seconds>(d);
	tp->tv_sec = s.count();
	tp->tv_usec = sc::duration_cast<sc::microseconds>(d - s).count();

	return 0;
}

uint32_t
timer_run (struct times *t, const uint32_t index, const uint32_t milliseconds)
{
	struct timeval tv;
	gettimeofday (&tv, NULL);

	uint64_t ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

	if (t[index].cur == 0L) {
		t[index].cur = ms;
		return 0;
	}

	uint64_t diff_time = ms - t[index].cur;

	t[index].turn += diff_time;
	if (t[index].turn >= milliseconds) {
		t[index].turn = 0L;
		t[index].cur = ms;
		return 1;
	}

	t[index].cur = ms;
	return 0;
}

void
timer_clear (struct times *t)
{
	free (t);
}

void 
timer_stop (struct times *t, const uint32_t index)
{
	t[index].cur = 0L;
	t[index].turn = 0L;
}
