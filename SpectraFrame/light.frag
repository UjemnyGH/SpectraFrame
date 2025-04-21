#version 450

layout(binding = 0) uniform sampler2D uAlbedo;

layout(location = 0) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 oCol;

void main() {
	oCol = texture(uAlbedo, vTextureCoordinates);
}