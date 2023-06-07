#include <cstdint>
#include <string>

namespace Utils {
	uint8_t* file_get_data (std::string& filename);
	uint8_t* file_get_game_data (uint64_t enum_pos, uint64_t& size, uint64_t group);
	int swap_little_big_engian (int num);
	uint64_t swap_little_big_engian_bigint (uint64_t num);
}
