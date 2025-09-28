#pragma once
#ifndef _SPECTRAFRAME_AUDIO_DEVICE_
#define _SPECTRAFRAME_AUDIO_DEVICE_

#include <AL/al.h>
#include <AL/alc.h>

namespace sf {
  class AudioDevice {
  public:
    static ALCdevice* sGlobalDevice;
    static ALCcontext* sGlobalContext;

    static void open();
    
    static void close();
  };
}

#endif