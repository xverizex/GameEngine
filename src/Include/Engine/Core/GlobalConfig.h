#ifndef GLOBAL_CONFIG_H
#define GLOBAL_CONFIG_H

#include <vector>
#include <Engine/Core/ILevel.h>

class GlobalConfig {
	public:
		virtual void init_shaders () = 0;
		virtual void init_screen () = 0;
		virtual void list_of_levels () = 0;
		virtual void entry_point () = 0;
		virtual void switch_level () = 0;

		ILevel* cur_level;

		std::vector<ILevel *> n_levels;
};

#endif
