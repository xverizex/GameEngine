#include <Engine/Core/Global.h>

namespace Global {

static ShaderManager* sm;

void set_singleton (ShaderManager* _sm)
{
	sm = _sm;
}

ShaderManager* get_singleton ()
{
	return sm;
}

}
