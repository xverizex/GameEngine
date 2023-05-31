#include <Engine/Core/ScreenManager.h>

void ScreenManager::set_screen_size (uint32_t _width, uint32_t _height, bool _is_fullscreen)
{
	width = _width;
	height = _height;
	is_fullscreen = _is_fullscreen;
	width_float = (float) _width;
	height_float = (float) _height;
}

void ScreenManager::get_screen_size (uint32_t& _width, uint32_t& _height, bool& _is_fullscreen)
{
	_width = width;
	_height = height;
	_is_fullscreen = is_fullscreen;
}
