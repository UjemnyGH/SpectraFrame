#pragma once
#ifndef _SPECTRAFRAME_RENDER_MESH_
#define _SPECTRAFRAME_RENDER_MESH_

#include "../Core/sfLogger.h"
#include <vector>

namespace sf {
	struct RenderMesh {
		std::vector<float> vertices;
		std::vector<float> normal;
		std::vector<float> color;
		std::vector<float> textureCoordinates;
		
		float textureId;
	};
}

#endif