#include <Engine/Utils/File.h>
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

}
