#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <Engine/Core/GlobalConfig.h>

enum TYPE_FONTS {
    MAIN_FONT_28,
    N_TYPE_FONTS
};

enum {
	TYPE_END_NONE,
	TYPE_END_GAME_OVER,
	TYPE_END_FINISH,
	N_TYPE_END
};

struct DataLevel {
	uint32_t difficult;
	uint32_t width;
	uint32_t height;
	uint32_t traps;
	uint32_t data;
	uint32_t left_data;
	uint8_t *map;
};

class GameConfig: public GlobalConfig {
	public:
		void init_shaders () override;
		void init_screen () override;
		void list_of_levels () override;
		void entry_point () override;
		void switch_level () override;
        void load_res (uint32_t i, bool with_shaders);
        void unload_res () override;





    static GameConfig* get_instance ();

		DataLevel data_level;

		uint32_t current_level = {0};

		uint32_t type_end = {TYPE_END_NONE};
};

#endif
