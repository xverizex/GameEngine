#pragma once
#include <stdint.h>
#ifdef __ANDROID__
#include "SDL.h"
#else
#include <SDL2/SDL.h>
#endif

#define FORMAT_MONO    0
#define FORMAT_STEREO  1

#include <cstdint>

class Sound {
public:
    virtual void init () = 0;
    virtual void set (uint32_t enum_res, int format, int freq) = 0;
    virtual void play () = 0;
    virtual void stop () = 0;
    virtual bool is_no_play () = 0;
};
