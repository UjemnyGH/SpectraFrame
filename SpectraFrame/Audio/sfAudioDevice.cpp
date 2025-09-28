#include "sfAudioDevice.h"
#include "../Core/sfLogger.h"

ALCdevice* sf::AudioDevice::sGlobalDevice = nullptr;
ALCcontext* sf::AudioDevice::sGlobalContext = nullptr;

void sf::AudioDevice::open() {
  sGlobalDevice = alcOpenDevice(nullptr);

  if(!sGlobalDevice) {
    SF_CLOG("[ERROR]: Cannot open AL device");
  }

  sGlobalContext = alcCreateContext(sGlobalDevice, nullptr);
  alcMakeContextCurrent(sGlobalContext);
}
    
void sf::AudioDevice::close() {
  alcMakeContextCurrent(nullptr);
  alcDestroyContext(sGlobalContext);
  alcCloseDevice(sGlobalDevice);
}