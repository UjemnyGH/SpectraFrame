#pragma once
#ifndef _SPECTRAFRAME_COMMON_
#define _SPECTRAFRAME_COMMON_

#include <cstdint>
#include <vector>
#include <string>

namespace sf {
	template <typename T, typename D>
	T loadValueAtOffsetLE(const D* const data, const uint64_t offset);

	template <typename T, typename D>
	T loadValueAtOffsetBE(const D* const data, const uint64_t offset);

	/**
	 * @brief Makes one vector out of 3 in fast way and return 3 offsets to data
	 * @tparam T 
	 * @param vertices 
	 * @param normals 
	 * @param textureCoordinates 
	 * @param outBuffer 
	 * @param normalsOffset
	 * @param textureCoordinatesOffset
	 */
	template <typename T>
	void makeOneBufferData(const std::vector<T>& vertices, const std::vector<T>& normals, const std::vector<T>& textureCoordinates, std::vector<T>& outBuffer);

	uint64_t readFile(const std::string& path, std::vector<uint8_t>& outBuffer);

	void writeFile(const std::string& path, const std::vector<uint8_t>& data);
}

#endif