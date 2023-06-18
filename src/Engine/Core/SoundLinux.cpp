#include <Engine/Core/SoundLinux.h>
#include <SDL2/SDL.h>
#include <Engine/Utils/File.h>
#include <Game/Core/ResEnum.h>

Sound_linux::~Sound_linux ()
{
	alDeleteBuffers (1, &buf);
	alDeleteSources (1, &source);
	delete[] data;
}

Sound_linux::Sound_linux ()
{
    size_b = 0;
}

static ALCdevice *device;
static ALCcontext *ctx;

void Sound_linux::init ()
{
	if (device == nullptr) {
    		device = alcOpenDevice (nullptr);
    		ctx = alcCreateContext (device, nullptr);
    		alcMakeContextCurrent (ctx);
	}
}



void Sound_linux::set (uint32_t enum_pos, int format, int freq)
{
    alGenBuffers (1, &buf);
    switch (format) {
	    case FORMAT_MONO:
		    format = AL_FORMAT_MONO16;
		    break;
	    case FORMAT_STEREO:
		    format = AL_FORMAT_STEREO16;
		    break;
    }

    size_t size;
    data = Utils::file_get_game_data (enum_pos, size, R_SOUNDS);
    alBufferData (buf, format, data, size, freq);

    int ret = alGetError ();

    uint32_t source;
    alGenSources (1, &source);
    alSourcei (source, AL_BUFFER, buf);

    buffer[0] = source;
}

void Sound_linux::play ()
{
    alSourcePlay (buffer[0]);
}

void Sound_linux::stop ()
{
    alSourceStop (buffer[0]);
}

bool Sound_linux::is_no_play ()
{
    int state;
    alGetSourcei (buffer[0], AL_SOURCE_STATE, &state);
    if (state == AL_STOPPED) return true;
    return false;
}
