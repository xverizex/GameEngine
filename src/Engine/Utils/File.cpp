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

uint8_t* file_get_game_data (uint64_t enum_pos, uint64_t& ss, uint64_t group_pos)
{
	ss = 0L;
	std::string filename = "assets/" GAME_DATA;

	SDL_RWops* io = SDL_RWFromFile (filename.c_str(), "rb");
	if (io != nullptr) {
		io->seek (io, group_pos * sizeof (uint64_t), SEEK_SET);
		uint64_t group = 0L;
		io->read (io, &group, sizeof (uint64_t), 1);
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
