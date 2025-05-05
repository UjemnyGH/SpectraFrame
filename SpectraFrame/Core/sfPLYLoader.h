#pragma once
#pragma once
#ifndef _SPECTRAFRAME_PLY_LOADER_
#define _SPECTRAFRAME_PLY_LOADER_

#include <vector>
#include <fstream>
#include <string>
#include <cstdint>

namespace sf {
    // Private enum
    enum MeshRawDataSourceInfo {
        MeshRawDataSourceInfo_vertices = 0x1,
        MeshRawDataSourceInfo_normals = 0x2,
        MeshRawDataSourceInfo_textureCoordinates = 0x4,
    };

    struct MeshRawData {
        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<float> textureCoordinates;
    };

    /**
     * @brief Load binary PLY mesh from memory (source)
     *
     * @param src source of loaded ply mesh
     * @param headerEnd end of ply header
     * @param verticesAmount amount of vertices in source
     * @param facesAmount amount of faces in source
     * @param meshSourceInfo what our ply mesh have
     * @return Mesh what you expect it to return else than "Mesh"
     */
    MeshRawData meshSourceBinary_PLY(std::string& src, std::size_t headerEnd, uint64_t verticesAmount, uint64_t facesAmount, uint8_t meshSourceInfo);

    /**
     * @brief Load PLY mesh from file
     *
     * @param path path to file
     * @return MeshRawData
     */
    MeshRawData loadPLYMesh(std::string path);

    void negateMeshNormals(MeshRawData* pMesh);

    void calculateNormalsBasedOnVertices(MeshRawData* pMesh);
}

#endif
