#include "sfPLYLoader.h"
#include "../Math/sfVector.h"
#include "sfCommon.h"

sf::MeshRawData getPLYMeshBinaryVertices(std::string& src, const size_t headerEnd, const uint64_t verticesAmount, const uint8_t multiplier, const uint8_t meshSourceInfo) {
    sf::MeshRawData tmp;

    // Loop over all vertices and load them to temporary mesh
    for (std::size_t i = 0; i < verticesAmount; i++) {
        // We need offset to first vertex
        const uint32_t vertexOffset = headerEnd + 11;

        if (meshSourceInfo & sf::MeshRawDataSourceInfo_vertices) {
            uint32_t ix = sf::loadValueAtOffsetLE<uint32_t, uint8_t>((uint8_t*)&src[0], vertexOffset + (i * multiplier * sizeof(float) + 0));
            uint32_t iy = sf::loadValueAtOffsetLE<uint32_t, uint8_t>((uint8_t*)&src[0], vertexOffset + (i * multiplier * sizeof(float) + 4));
            uint32_t iz = sf::loadValueAtOffsetLE<uint32_t, uint8_t>((uint8_t*)&src[0], vertexOffset + (i * multiplier * sizeof(float) + 8));

            // Recast our 3 vertices to floats (more usable than integers)
            tmp.vertices.push_back(*reinterpret_cast<float*>(&ix));
            tmp.vertices.push_back(*reinterpret_cast<float*>(&iy));
            tmp.vertices.push_back(*reinterpret_cast<float*>(&iz));
        }

        if (meshSourceInfo & (sf::MeshRawDataSourceInfo_normals | sf::MeshRawDataSourceInfo_textureCoordinates)) {
            // Same as vertices
            uint32_t inx = sf::loadValueAtOffsetLE<uint32_t, uint8_t>((uint8_t*)&src[0], vertexOffset + (i * multiplier * sizeof(float) + 12));
            uint32_t iny = sf::loadValueAtOffsetLE<uint32_t, uint8_t>((uint8_t*)&src[0], vertexOffset + (i * multiplier * sizeof(float) + 16));
            uint32_t inz = sf::loadValueAtOffsetLE<uint32_t, uint8_t>((uint8_t*)&src[0], vertexOffset + (i * multiplier * sizeof(float) + 20));

            uint32_t is = sf::loadValueAtOffsetLE<uint32_t, uint8_t>((uint8_t*)&src[0], vertexOffset + (i * multiplier * sizeof(float) + 24));
            uint32_t it = sf::loadValueAtOffsetLE<uint32_t, uint8_t>((uint8_t*)&src[0], vertexOffset + (i * multiplier * sizeof(float) + 28));

            tmp.normals.push_back(*reinterpret_cast<float*>(&inx));
            tmp.normals.push_back(*reinterpret_cast<float*>(&iny));
            tmp.normals.push_back(*reinterpret_cast<float*>(&inz));
            tmp.textureCoordinates.push_back(*reinterpret_cast<float*>(&is));
            tmp.textureCoordinates.push_back(*reinterpret_cast<float*>(&it));
        }
        else if (meshSourceInfo & sf::MeshRawDataSourceInfo_normals && !(meshSourceInfo & sf::MeshRawDataSourceInfo_textureCoordinates)) {
            // Same as vertices
            uint32_t inx = sf::loadValueAtOffsetLE<uint32_t, uint8_t>((uint8_t*)&src[0], vertexOffset + (i * multiplier * sizeof(float) + 12));
            uint32_t iny = sf::loadValueAtOffsetLE<uint32_t, uint8_t>((uint8_t*)&src[0], vertexOffset + (i * multiplier * sizeof(float) + 16));
            uint32_t inz = sf::loadValueAtOffsetLE<uint32_t, uint8_t>((uint8_t*)&src[0], vertexOffset + (i * multiplier * sizeof(float) + 20));

            tmp.normals.push_back(*reinterpret_cast<float*>(&inx));
            tmp.normals.push_back(*reinterpret_cast<float*>(&iny));
            tmp.normals.push_back(*reinterpret_cast<float*>(&inz));
        }
        else if (!(meshSourceInfo & sf::MeshRawDataSourceInfo_normals) && meshSourceInfo & sf::MeshRawDataSourceInfo_textureCoordinates) {
            // Same as vertices
            uint32_t is = sf::loadValueAtOffsetLE<uint32_t, uint8_t>((uint8_t*)&src[0], vertexOffset + (i * multiplier * sizeof(float) + 12));
            uint32_t it = sf::loadValueAtOffsetLE<uint32_t, uint8_t>((uint8_t*)&src[0], vertexOffset + (i * multiplier * sizeof(float) + 16));

            tmp.textureCoordinates.push_back(*reinterpret_cast<float*>(&is));
            tmp.textureCoordinates.push_back(*reinterpret_cast<float*>(&it));
        }
    }

    return tmp;
}

sf::MeshRawData sf::meshSourceBinary_PLY(std::string& src, std::size_t headerEnd, uint64_t verticesAmount, uint64_t facesAmount, uint8_t meshSourceInfo) {
    MeshRawData result;

    uint8_t multiplier = 0;

    // Check that we have in mesh and add it to multiplier
    if (meshSourceInfo & MeshRawDataSourceInfo_vertices) multiplier += 3;
    if (meshSourceInfo & MeshRawDataSourceInfo_normals) multiplier += 3;
    if (meshSourceInfo & MeshRawDataSourceInfo_textureCoordinates) multiplier += 2;

    // If we have nothing add 1 to crash program
    if (multiplier == 0) multiplier += 1;

    MeshRawData tmp = getPLYMeshBinaryVertices(src, headerEnd, verticesAmount, multiplier, meshSourceInfo);

    uint32_t index = 0;

    for (std::size_t i = 0; i < facesAmount; i++) {
        const uint32_t facesOffset = headerEnd + 11 + (sizeof(float) * multiplier * verticesAmount);

        // Get number of indices to join
        uint8_t indicesAmount = (uint8_t)src[facesOffset + index];

        if (indicesAmount == 3) {
            // Get indices
            uint32_t indices[3] = { 
                sf::loadValueAtOffsetLE<uint32_t, uint8_t>((uint8_t*)&src[0], facesOffset + (index + 1)),
                sf::loadValueAtOffsetLE<uint32_t, uint8_t>((uint8_t*)&src[0], facesOffset + (index + 5)),
                sf::loadValueAtOffsetLE<uint32_t, uint8_t>((uint8_t*)&src[0], facesOffset + (index + 9)) 
            };

            index += 13;

            // Make vertices
            for (uint8_t i = 0; i < 3; i++) {
                if (meshSourceInfo & MeshRawDataSourceInfo_vertices) {
                    for(uint8_t j = 0; j < 3; j++)
                        result.vertices.push_back(tmp.vertices[indices[i] * 3 + j]);
                }

                if (meshSourceInfo & MeshRawDataSourceInfo_normals) {
                    for (uint8_t j = 0; j < 3; j++)
                        result.vertices.push_back(tmp.vertices[indices[i] * 3 + j]);
                }

                if (meshSourceInfo & MeshRawDataSourceInfo_textureCoordinates) {
                    for(uint8_t j = 0; j < 2; j++)
                        result.textureCoordinates.push_back(tmp.textureCoordinates[indices[i] * 2 + j]);
                }
            }
        }
        else if (indicesAmount == 4) {
            uint32_t indices[4] = {
                sf::loadValueAtOffsetLE<uint32_t, uint8_t>((uint8_t*)&src[0], facesOffset + (index + 1)),
                sf::loadValueAtOffsetLE<uint32_t, uint8_t>((uint8_t*)&src[0], facesOffset + (index + 5)),
                sf::loadValueAtOffsetLE<uint32_t, uint8_t>((uint8_t*)&src[0], facesOffset + (index + 9)),
                sf::loadValueAtOffsetLE<uint32_t, uint8_t>((uint8_t*)&src[0], facesOffset + (index + 13))
            };

            index += 17;

            for (uint8_t i = 0; i < 3; i++) {
                if (meshSourceInfo & MeshRawDataSourceInfo_vertices) {
                    for (uint8_t j = 0; j < 3; j++)
                        result.vertices.push_back(tmp.vertices[indices[i] * 3 + j]);
                }

                if (meshSourceInfo & MeshRawDataSourceInfo_normals) {
                    for (uint8_t j = 0; j < 3; j++)
                        result.vertices.push_back(tmp.vertices[indices[i] * 3 + j]);
                }

                if (meshSourceInfo & MeshRawDataSourceInfo_textureCoordinates) {
                    for (uint8_t j = 0; j < 2; j++)
                        result.textureCoordinates.push_back(tmp.textureCoordinates[indices[i] * 2 + j]);
                }
            }

            for (uint8_t i = 0; i < 3; i++) {
                if (meshSourceInfo & MeshRawDataSourceInfo_vertices) {
                    for (uint8_t j = 0; j < 3; j++)
                        result.vertices.push_back(tmp.vertices[indices[i + (i > 0 ? 1 : 0)] * 3 + j]);
                }

                if (meshSourceInfo & MeshRawDataSourceInfo_normals) {
                    for (uint8_t j = 0; j < 3; j++)
                        result.vertices.push_back(tmp.vertices[indices[i + (i > 0 ? 1 : 0)] * 3 + j]);
                }

                if (meshSourceInfo & MeshRawDataSourceInfo_textureCoordinates) {
                    for (uint8_t j = 0; j < 2; j++)
                        result.textureCoordinates.push_back(tmp.textureCoordinates[indices[i + (i > 0 ? 1 : 0)] * 2 + j]);
                }
            }
        }
    }

    return result;
}

/**
* @brief Load PLY mesh from file
*
* @param path path to file
* @return MeshRawData
*/
sf::MeshRawData sf::loadPLYMesh(std::string path) {
    std::ifstream file;
    file.open(path, std::ios::binary | std::ios::ate);

    uint32_t len = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string src;
    src.resize(len);

    file.read((char*)src.data(), src.size());

    file.close();

    std::size_t plyMV = src.find("ply");

    if (plyMV == std::string::npos) return MeshRawData();

    std::size_t formatOff = src.find("format binary_little_endian");
    std::size_t verticesAmountOff = src.find("element vertex");
    std::size_t verticesOff = src.find("property float x");
    std::size_t normalsOff = src.find("property float nx");
    std::size_t textureCoordsOff = src.find("property float s");
    std::size_t facesAmountOff = src.find("element face");
    std::size_t headerEnd = src.find("end_header");

    uint64_t verticesAmount = std::stoi(std::string(src.begin() + verticesAmountOff + 15, src.begin() + src.find_first_of('\n', verticesAmountOff)));
    uint64_t facesAmount = std::stoi(std::string(src.begin() + facesAmountOff + 13, src.begin() + src.find_first_of('\n', facesAmountOff)));

    if (formatOff != std::string::npos) {
        return meshSourceBinary_PLY(src, headerEnd, verticesAmount, facesAmount, (verticesOff != std::string::npos ? MeshRawDataSourceInfo_vertices : 0) | (normalsOff != std::string::npos ? MeshRawDataSourceInfo_normals : 0) | (textureCoordsOff != std::string::npos ? MeshRawDataSourceInfo_textureCoordinates : 0));
    }

    return MeshRawData();
}

void sf::negateMeshNormals(MeshRawData* pMesh) {
    std::vector<float> tempNormals;
    // Reserve memory to avoid allocating it every push_back call
    tempNormals.reserve(pMesh->vertices.size());

    for (std::size_t i = 0; i < pMesh->normals.size() / 3; i++) {
        FVec3 normal = FVec3(pMesh->normals[i * 3 + 0], pMesh->normals[i * 3 + 1], pMesh->normals[i * 3 + 2]).negate();

        tempNormals.push_back(normal.x);
        tempNormals.push_back(normal.y);
        tempNormals.push_back(normal.z);
    }

    pMesh->normals.clear();
    std::copy(tempNormals.begin(), tempNormals.end(), std::back_inserter(pMesh->normals));
}

void sf::calculateNormalsBasedOnVertices(MeshRawData* pMesh) {
    std::vector<float> tempNormals;
    tempNormals.reserve(pMesh->vertices.size());

    for (std::size_t i = 0; i < pMesh->vertices.size() / 9; i++) {
        FVec3 a = FVec3(pMesh->vertices[i * 9 + 0], pMesh->vertices[i * 9 + 1], pMesh->vertices[i * 9 + 2]);
        FVec3 b = FVec3(pMesh->vertices[i * 9 + 3], pMesh->vertices[i * 9 + 4], pMesh->vertices[i * 9 + 5]);
        FVec3 c = FVec3(pMesh->vertices[i * 9 + 6], pMesh->vertices[i * 9 + 7], pMesh->vertices[i * 9 + 8]);

        FVec3 normal = (b - a).cross(c - a).normalize(); // FVec3::PlaneNormal(a, b, c);

        tempNormals.push_back(normal.x);
        tempNormals.push_back(normal.y);
        tempNormals.push_back(normal.z);

        tempNormals.push_back(normal.x);
        tempNormals.push_back(normal.y);
        tempNormals.push_back(normal.z);

        tempNormals.push_back(normal.x);
        tempNormals.push_back(normal.y);
        tempNormals.push_back(normal.z);
    }

    pMesh->normals.clear();
    std::copy(tempNormals.begin(), tempNormals.end(), std::back_inserter(pMesh->normals));
}