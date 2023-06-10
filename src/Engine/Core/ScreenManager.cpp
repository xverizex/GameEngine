#include <Engine/Core/ScreenManager.h>
#include <assdl.h>

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
#ifdef __ANDROID__
	SDL_GetCurrentDisplayMode (index, &mode);
#else
	SDL_GetDesktopDisplayMode (index, &mode);
#endif

#if WIN32
	set_screen_size (mode.w, mode.h, false);
#else
	set_screen_size (mode.w, mode.h, true);
#endif
}
