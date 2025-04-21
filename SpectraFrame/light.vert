#version 450

vec2 g_vertices[6] = {
	vec2(-1.0, -1.0),
	vec2(1.0, -1.0),
	vec2(-1.0, 1.0),

	vec2(1.0, -1.0),
	vec2(-1.0, 1.0),
	vec2(1.0, 1.0)
};

vec2 g_textureCoordinates[6] = {
	vec2(0.0, 0.0),
	vec2(1.0, 0.0),
	vec2(0.0, 1.0),

	vec2(1.0, 0.0),
	vec2(0.0, 1.0),
	vec2(1.0, 1.0)
};

layout(location = 0) out vec2 vTextureCoordinates;

void main() {
	gl_Position = vec4(g_vertices[gl_VertexIndex], 0.0, 1.0);
	vTextureCoordinates = g_textureCoordinates[gl_VertexIndex];
}