#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <Engine/Core/GlobalConfig.h>

enum {
	SHADER_3D_WITHOUT_LIGHT,
	N_SHADERS
};

class GameConfig: public GlobalConfig {
	public:
		void init_shaders () override;
};

#endif
