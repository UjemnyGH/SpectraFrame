#include "sfCommon.h"

template <typename T, typename D>
T sf::loadValueAtOffsetLE(const D* const data, const uint64_t offset) {
	T result = static_cast<T>(0);

	for (uint32_t i = 0; i < sizeof(T); i++) {
		result |= static_cast<T>(static_cast<D>(data[offset + i])) << (8 * sizeof(D) * i);
	}

	return result;
}

template <typename T, typename D>
T sf::loadValueAtOffsetBE(const D* const data, const uint64_t offset) {
	T result;

	for (uint32_t i = 0; i < sizeof(T); i++) {
		result |= static_cast<T>(static_cast<D>(data[offset + i])) << (8 * sizeof(D) * (sizeof(T) - 1 - i));
	}

	return result;
}