#include <Engine/Core/Global.h>

namespace Global {

template<typename T>
static T* m;

template<typename T>
void set_singleton (T* _sm)
{
	m<T> = _sm;
}

template<typename T>
T* get_singleton ()
{
	return m<T>;
}

template void set_singleton<ShaderManager>(ShaderManager*);
template ShaderManager* get_singleton<ShaderManager>();

}
