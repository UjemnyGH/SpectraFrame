#include "sfAudioEmitter.h"
#include "../Core/sfLogger.h"

void sf::AudioEmitter::AudioWav::loadFromWAV(const std::string& path) {
  std::ifstream wav(path, std::ios::binary | std::ios::ate);

  if(wav.bad() && !wav.is_open()) {
    SF_CLOG("[ERROR]: Cannot open " << path << " waveform file");

    return;
  }

  size_t len = wav.tellg();
  wav.seekg(0, std::ios::beg);

  std::vector<uint8_t> temp(len);
  wav.read((char*)temp.data(), temp.size());

  wav.close();

  subchunk1Id = (uint32_t)temp[12] << 24 | (uint32_t)temp[13] << 16 | (uint32_t)temp[14] << 8 | (uint32_t)temp[15];
  subchunk1Size = (uint32_t)temp[19] << 24 | (uint32_t)temp[18] << 16 | (uint32_t)temp[17] << 8 | (uint32_t)temp[16];
  audioFormat = (uint16_t)temp[21] << 8 | (uint16_t)temp[20];
  channels = (uint16_t)temp[23] << 8 | (uint16_t)temp[22];
  samplerate = (uint32_t)temp[27] << 24 | (uint32_t)temp[26] << 16 | (uint32_t)temp[25] << 8 | (uint32_t)temp[24];
  byterate = (uint32_t)temp[31] << 24 | (uint32_t)temp[30] << 16 | (uint32_t)temp[29] << 8 | (uint32_t)temp[28];
  blockAlign = (uint16_t)temp[33] << 8 | (uint16_t)temp[32];
  bitsPerSample = (uint16_t)temp[35] << 8 | (uint16_t)temp[34];
  subchunk2Id = (uint32_t)temp[36] << 24 | (uint32_t)temp[37] << 16 | (uint32_t)temp[38] << 8 | (uint32_t)temp[39];
  subchunk2Size = (uint32_t)temp[43] << 24 | (uint32_t)temp[42] << 16 | (uint32_t)temp[41] << 8 | (uint32_t)temp[40];

  data.resize(subchunk2Size);
  data.assign(temp.begin() + 44, temp.end());
}

void sf::AudioEmitter::setListenerPosition(const FVec3& position) {
  alListener3f(AL_POSITION, position.x, position.y, position.z);
}

void sf::AudioEmitter::setListenerVelocity(const FVec3& velocity) {
  alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void sf::AudioEmitter::setListenerOrientation(const FVec3& at, const FVec3& up) {
  float orientation[] = { at.x, at.y, at.z, up.x, up.y, up.z };
  alListenerfv(AL_ORIENTATION, orientation);
}

sf::AudioEmitter::~AudioEmitter() {
  destroy();
}

sf::AudioEmitter& sf::AudioEmitter::loadWAV(const std::string& path) {
  AudioWav wav;
  wav.loadFromWAV(path);

  // Format is calculated based on wav.channel which is 1 for mono and 2 for stereo and wav.bitPerSample which is 8 for 8bit and 16 for 16bit 
  uint32_t format = AL_FORMAT_MONO8 + ((wav.channels - 1) * 2) + ((wav.bitsPerSample / 8) - 1);

  if(!mAudioBuffer)
    alGenBuffers(1, &mAudioBuffer);
  alBufferData(mAudioBuffer, format, wav.data.data(), wav.data.size(), wav.samplerate);

  if(!mAudioSource)
    alGenSources(1, &mAudioSource);
  alSourcei(mAudioSource, AL_BUFFER, mAudioBuffer);

  return *this;
}

sf::AudioEmitter& sf::AudioEmitter::setAudioPosition(const FVec3& position) {
  alSource3f(mAudioSource, AL_POSITION, position.x, position.y, position.z);

  return *this;
}

sf::AudioEmitter& sf::AudioEmitter::setPitch(const float pitch) {
  alSourcef(mAudioSource, AL_PITCH, pitch);
  
  return *this;
}

sf::AudioEmitter& sf::AudioEmitter::setGain(const float gain) {
  alSourcef(mAudioSource, AL_GAIN, gain);

  return *this;
}

sf::AudioEmitter& sf::AudioEmitter::play() {
  alSourcePlay(mAudioSource);

  return *this;
}

sf::AudioEmitter& sf::AudioEmitter::rewind() {
  alSourceRewind(mAudioSource);

  return *this;
}

sf::AudioEmitter& sf::AudioEmitter::loop(bool loop) {
  alSourcei(mAudioSource, AL_LOOPING, loop);

  return *this;
}

sf::AudioEmitter& sf::AudioEmitter::pause() {
  alSourcePause(mAudioSource);

  return *this;
}

sf::AudioEmitter& sf::AudioEmitter::stop() {
  alSourceStop(mAudioSource);

  return *this;
}

sf::AudioEmitter& sf::AudioEmitter::destroy() {
  if(mAudioSource)
    alDeleteSources(1, &mAudioSource);

  mAudioSource = 0;
  
  if(mAudioBuffer)
    alDeleteBuffers(1, &mAudioBuffer);

  mAudioBuffer = 0;

  return *this;
}