#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <Engine/Core/GlobalConfig.h>

class GameConfig: public GlobalConfig {
	public:
		void init_shaders () override;
		void init_screen () override;
};

#endif
