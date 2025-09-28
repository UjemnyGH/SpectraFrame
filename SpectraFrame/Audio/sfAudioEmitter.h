#pragma once
#ifndef _SPECTRAFRAME_AUDIO_EMITTER_
#define _SPECTRAFRAME_AUDIO_EMITTER_

#include <AL/al.h>
#include <AL/alc.h>
#include <fstream>
#include <filesystem>
#include "../Math/sfVector.h"

namespace sf {
  class AudioEmitter {
  private:
    uint32_t mAudioBuffer;
    uint32_t mAudioSource;

    enum WavChannel {
      WavChannel_Mono = 1,
      WavChannel_Stereo = 2
    };

    struct AudioWav {
      uint32_t subchunk1Id;
      uint32_t subchunk1Size;
      uint16_t audioFormat;
      uint16_t channels;
      uint32_t samplerate;
      uint32_t byterate;
      uint16_t blockAlign;
      uint16_t bitsPerSample;
      uint32_t subchunk2Id;
      uint32_t subchunk2Size;
      // Samples depends on bitsPerSample and channels
      std::vector<uint8_t> data;

      void loadFromWAV(const std::string& path);
    };

  public:
    static void setListenerPosition(const FVec3& position);

    static void setListenerVelocity(const FVec3& velocity);

    static void setListenerOrientation(const FVec3& at, const FVec3& up);

    ~AudioEmitter();

    AudioEmitter& loadWAV(const std::string& path);

    AudioEmitter& setAudioPosition(const FVec3& position);

    AudioEmitter& setPitch(const float pitch);

    AudioEmitter& setGain(const float gain);

    AudioEmitter& play();

    AudioEmitter& rewind();

    AudioEmitter& loop(bool loop);

    AudioEmitter& pause();

    AudioEmitter& stop();

    AudioEmitter& destroy();
  };
}

#endif