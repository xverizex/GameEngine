#include <Engine/Utils/File.h>
#include <Game/Core/Defines.h>
#include <Game/Core/ResEnum.h>
#include <SDL2/SDL.h>

namespace Utils {

uint8_t* file_get_data (std::string& filename)
{
	filename = "assets/" + filename;

	SDL_RWops* io = SDL_RWFromFile (filename.c_str(), "rb");
	if (io != nullptr) {
		uint64_t size = io->size (io);
		uint8_t* data = new uint8_t[size + 1];
		io->read (io, data, size, 1);
		io->close (io);
		data[size] = 0;
		return data;
	}

	return nullptr;
}

uint64_t swap_little_big_engian_bigint (uint64_t num)
{
        return
                (((num >> 56) & 0xff) |
                 ((num >> 40) & 0xff00) |
                 ((num >> 24) & 0xff0000) |
                 ((num >>  8) & 0xff000000) |
                 ((num <<  8) & 0xff00000000) |
                 ((num << 24) & 0xff0000000000) |
                 ((num << 40) & 0xff000000000000) |
                 ((num << 56) & 0xff00000000000000)
                );
}

int swap_little_big_engian (int num)
{
        return (((num >> 24) & 0xff) | ((num << 8) & 0xff0000) | ((num >> 8) & 0xff00) | ((num << 24) & 0xff000000));
}

uint8_t* file_get_game_data (uint64_t enum_pos, uint64_t& ss, uint64_t group_pos)
{
	ss = 0L;
	std::string filename = "assets/" GAME_DATA;

	SDL_RWops* io = SDL_RWFromFile (filename.c_str(), "rb");
	if (io != nullptr) {
		static uint64_t lb = 0L;
		if (lb == 0L) {
			io->seek (io, 0, SEEK_SET);
			io->read (io, &lb, sizeof (uint64_t), 1);
		}
		io->seek (io, sizeof (uint64_t) + group_pos * sizeof (uint64_t), SEEK_SET);
		uint64_t group = 0L;
		io->read (io, &group, sizeof (uint64_t), 1);
		if (lb != 1)
			group = swap_little_big_engian_bigint (group);

		io->seek (io, group + enum_pos * sizeof (uint64_t), SEEK_SET);
		uint64_t pos = 0L;
		io->read (io, &pos, sizeof (uint64_t), 1);
		io->seek (io, pos, SEEK_SET);
		uint64_t size;
		io->read (io, &size, sizeof (uint64_t), 1);
		ss = size;
		uint8_t* data = new uint8_t[size + 1];
		io->read (io, data, size, 1);
		io->close (io);
		data[size] = 0;
		return data;
	}

	return nullptr;
}

}
