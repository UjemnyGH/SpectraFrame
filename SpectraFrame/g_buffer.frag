#version 450

layout(binding = 1) uniform sampler2D uTexture;

layout(location = 0) in vec4 vCol;
layout(location = 1) in vec2 vTexCoords;

layout(location = 0) out vec4 oCol;

void main() {
	oCol = texture(uTexture, vTexCoords) * vCol;
}