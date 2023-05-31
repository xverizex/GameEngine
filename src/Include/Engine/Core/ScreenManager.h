#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <cstdint>

class ScreenManager {
	public:
		void set_screen_size (uint32_t width, uint32_t height, bool is_fullscreen);
		void get_screen_size (uint32_t& width, uint32_t& height, bool& is_fullscreen);
	protected:
		uint32_t width;
		uint32_t height;
		bool is_fullscreen;
};

#endif
