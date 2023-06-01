#ifndef ILEVEL_H
#define ILEVEL_H
#include <cstdint>
#include <Engine/Core/Global.h>

class GameConfig;

class ILevel {
	public:
		ILevel () {
			cfg = Global::get_singleton<GameConfig> ();
		}
		virtual void load_assets () = 0;
		virtual void unload_assets () = 0;
		virtual void tick () = 0;
		virtual void mouse_click (int x, int y) = 0;
		virtual void render () = 0;

		uint32_t new_level;

		GameConfig* cfg;
};


#endif
