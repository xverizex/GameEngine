#ifndef ISHADER_H
#define ISHADER_H
#include <stdint.h>

class IShader {
	public:
		virtual void init () = 0;
		virtual void render(void *_data) = 0;
};


#endif
