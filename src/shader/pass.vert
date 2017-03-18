R"(
#version 330

layout(location = 0) in vec2 xin;
out vec2 gxin;
out float index;

uniform float scale;
void main () {
	gxin = xin * scale;
	index = float(gl_VertexID);
}
)"
