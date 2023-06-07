#ifndef ILEVEL_H
#define ILEVEL_H
#include <cstdint>
#include <SDL2/SDL.h>

class GameConfig;

class ILevel {
	public:
		ILevel ();
		virtual void clear_screen () = 0;
		virtual void load_assets () = 0;
		virtual void unload_assets () = 0;
		virtual void tick () = 0;
		virtual void mouse_click (int x, int y) = 0;
		virtual void key_click (SDL_Keysym key) = 0;
		virtual void render () = 0;

		uint32_t new_level;

		GameConfig* cfg;
};


#endif
