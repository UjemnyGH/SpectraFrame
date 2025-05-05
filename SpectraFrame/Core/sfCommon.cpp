#include "sfCommon.h"
#include <iterator>
#include <string>
#include <fstream>

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

template <typename T>
void sf::makeOneBufferData(const std::vector<T>& vertices, const std::vector<T>& normals, const std::vector<T>& textureCoordinates, std::vector<T>& outBuffer) {
	outBuffer.clear();
	outBuffer.resize(vertices.size() + normals.size() + textureCoordinates.size());

	// Size of vertices should be always 3 as for 3 axis (XYZ)
	for (uint64_t i = 0; i < vertices.size() / 3; i++) {
		outBuffer[i * 8 + 0] = vertices[i * 3 + 0];
		outBuffer[i * 8 + 1] = vertices[i * 3 + 1];
		outBuffer[i * 8 + 2] = vertices[i * 3 + 2];

		outBuffer[i * 8 + 3] = normals[i * 3 + 0];
		outBuffer[i * 8 + 4] = normals[i * 3 + 1];
		outBuffer[i * 8 + 5] = normals[i * 3 + 2];

		outBuffer[i * 8 + 6] = textureCoordinates[i * 2 + 0];
		outBuffer[i * 8 + 7] = textureCoordinates[i * 2 + 1];
	}
}

uint64_t sf::readFile(const std::string& path, std::vector<uint8_t>& outBuffer) {
	std::ifstream fileIn(path, std::ios::binary | std::ios::ate);

	uint64_t length = fileIn.tellg();
	fileIn.seekg(0, std::ios::beg);

	outBuffer.resize(length);

	fileIn.read((char*)&outBuffer[0], length);

	fileIn.close();

	return length;
}

void sf::writeFile(const std::string& path, const std::vector<uint8_t>& data) {
	std::ofstream fileOut(path, std::ios::binary);

	fileOut.write((char*)&data[0], data.size());

	fileOut.close();
}