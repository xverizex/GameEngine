#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <Engine/Core/GlobalConfig.h>

class GameConfig: public GlobalConfig {
	public:
		void init_shaders () override;
		void init_screen () override;
		void list_of_levels () override;
		void entry_point () override;
		void switch_level () override;

		static GameConfig* get_instance ();
};

#endif
