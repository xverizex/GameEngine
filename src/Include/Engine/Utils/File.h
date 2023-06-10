#include <cstdint>
#include <string>
#include <assdl.h>
#include <string>

namespace Utils {
	uint8_t* file_get_data (std::string& filename);
	uint8_t* file_get_game_data (uint64_t enum_pos, uint64_t& size, uint64_t group);
	int swap_little_big_engian (int num);
	uint64_t swap_little_big_engian_bigint (uint64_t num);
	SDL_RWops* get_io_file (std::string filename, std::string mode);
	SDL_RWops* get_io_file_local (std::string filename, std::string mode);
}
