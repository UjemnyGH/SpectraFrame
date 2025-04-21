#version 450

layout(binding = 0) uniform PVT {
	mat4 uProjection;
	mat4 uView;
	mat4 uTransform;
} pvt; 

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec4 iCol;
layout(location = 2) in vec2 iTexCoords;

layout(location = 0) out vec4 vCol;
layout(location = 1) out vec2 vTexCoords;

void main() {
	gl_Position = pvt.uProjection * pvt.uView * pvt.uTransform * vec4(iPos, 1.0);

	vCol = iCol;
	vTexCoords = iTexCoords;
}