#include <cstdint>
#include <string>

namespace Utils {
	uint8_t* file_get_data (std::string& filename);
	uint8_t* file_get_game_data (uint64_t enum_pos, uint64_t& size, uint64_t group);
}
