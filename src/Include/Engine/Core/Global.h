#ifndef GLOBAL_H
#define GLOBAL_H
#include <Engine/Core/ShaderManager.h>

namespace Global {

	void set_singleton (ShaderManager* sm);

	ShaderManager* get_singleton ();
}

#endif
