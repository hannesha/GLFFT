R"(
#version 150
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFramebuffer;

uniform vec2 offset;
void main()
{
	vec2 offset_red = -offset * 0.5;
	vec2 offset_cyan = offset * 0.5;

	vec4 sum = vec4(0.0);
	sum += texture(texFramebuffer, Texcoord + offset_red) * vec4(1.0, 0.0, 0.0, 1.0);
	sum += texture(texFramebuffer, Texcoord + offset_cyan) * vec4(0.0, 1.0, 1.0, 1.0);
	outColor = sum;
}
)"
