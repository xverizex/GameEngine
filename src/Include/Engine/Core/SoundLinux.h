//
// Created by cf on 5/14/22.
//

#ifndef SOUND_LINUX_H
#define SOUND_LINUX_H

#include <cstdint>
#include <Engine/Core/Sound.h>

#include <AL/al.h>
#include <AL/alc.h>

class Sound_linux: public Sound {
	public:
    Sound_linux ();
    ~Sound_linux ();
    void init () override;
    void set (uint32_t enum_res, int format, int freq) override;
    void play () override;
    void stop () override;
    bool is_no_play () override;

    uint32_t buffer[1];
    uint32_t size_b;
    uint8_t* data;
	uint32_t source;
	uint32_t buf;
};


#endif //ONE_IN_TAIGA_SOUND_LINUX_H
