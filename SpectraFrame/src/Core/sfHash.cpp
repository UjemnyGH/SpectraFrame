#include "sfHash.h"

sf::hash_t sf::generateSFHash(const void* const data, const uint32_t size, const hash_t seed) {
	const hash_t primeConst = (0x9E3779B185EBCA87 ^ seed) * seed;

	hash_t precalc = ((hash_t)((uint8_t*)data)[0]) * primeConst;

	uint32_t currentPrecalcShift = 0;

	for (uint32_t i = 1; i < size; i++) {
		precalc ^= primeConst * (((hash_t)((uint8_t*)data)[i]) << currentPrecalcShift);

		currentPrecalcShift += ((uint32_t)((uint8_t*)data)[i] & 0x7);

		currentPrecalcShift %= 64;
	}

	precalc ^= precalc >> 33;
	precalc *= 0xF1C9BCA1F42D5D13;
	precalc ^= precalc >> 27;
	precalc *= 0xF6A3E8A3E4C18E91;
	precalc ^= precalc >> 19;
	precalc *= 0xC6A4A7935BD1E995;
	precalc ^= precalc << 12;
	precalc *= 0xA897A0C93DCE2B57;
	precalc ^= precalc >> 39;
	precalc *= 0xD2E2F97D1A7A1F45;
	precalc ^= precalc << 41;

	return precalc;
}