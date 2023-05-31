#ifndef GLOBAL_H
#define GLOBAL_H
#include <Engine/Core/ShaderManager.h>

namespace Global {

	template<typename T>
	void set_singleton (T* sm);

	template<typename T>
	T* get_singleton ();
}

#endif
