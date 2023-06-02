#include <Engine/Core/ScreenManager.h>
#include <SDL2/SDL.h>

ScreenManager* ScreenManager::get_instance ()
{
	static ScreenManager* instance = new ScreenManager ();

	return instance;
}

void ScreenManager::set_screen_size (uint32_t _width, uint32_t _height, bool _is_fullscreen)
{
	width = _width;
	height = _height;
	is_fullscreen = _is_fullscreen;
	width_float = static_cast<float>(_width);
	height_float = static_cast<float>(_height);
}

void ScreenManager::set_desktop_size (uint32_t index)
{
	SDL_DisplayMode mode;
	SDL_GetDesktopDisplayMode (index, &mode);

	set_screen_size (mode.w, mode.h, true);
}
