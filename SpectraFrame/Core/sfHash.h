#pragma once
#ifndef _SPECTRAFRAME_HASH_
#define _SPECTRAFRAME_HASH_

#include <cstdint>

namespace sf {
  typedef unsigned long long hash_t;
  
  hash_t generateSFHash(const void* const data, const uint32_t size, const hash_t seed = 0xA55600DB00B5600D);
}

#endif
