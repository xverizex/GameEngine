#ifndef ILEVEL_H
#define ILEVEL_H
#include <cstdint>
#include <assdl.h>

class GameConfig;

class ILevel {
	public:
		ILevel ();
		virtual void clear_screen ();
		virtual void load_assets ();
		virtual void unload_assets ();
		virtual void tick ();
        virtual void load_stats ();
        virtual void save_stats ();
        virtual void load_level ();
        virtual void unload_level ();
		virtual void mouse_click_down (int x, int y);
		virtual void mouse_click_up (int x, int y);
		virtual void mouse_motion (int x, int y);
		virtual void key_click (SDL_Keysym key);
		virtual void render ();

		uint32_t new_level;

        bool loaded_level = {false};
        bool assets_loaded = {false};
		GameConfig* cfg;
        bool active = {false};
};


#endif
