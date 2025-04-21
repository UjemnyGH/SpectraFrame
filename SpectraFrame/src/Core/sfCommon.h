#pragma once
#ifndef _SPECTRAFRAME_COMMON_
#define _SPECTRAFRAME_COMMON_

#include <cstdint>

namespace sf {
	template <typename T, typename D>
	T loadValueAtOffsetLE(const D* const data, const uint64_t offset);

	template <typename T, typename D>
	T loadValueAtOffsetBE(const D* const data, const uint64_t offset);
}

#endif