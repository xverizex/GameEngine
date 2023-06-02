#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <cstdint>

class ScreenManager {
	public:
		static ScreenManager* get_instance ();
		void set_screen_size (uint32_t width, uint32_t height, bool is_fullscreen);
		void set_desktop_size (uint32_t indx);

		uint32_t width;
		uint32_t height;
		float width_float;
		float height_float;
		bool is_fullscreen;
};

#endif
