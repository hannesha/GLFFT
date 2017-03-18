R"(
#version 150

layout (lines_adjacency) in;
layout (triangle_strip, max_vertices = 4) out;

out vec4 color;

uniform float width;

void main () {
	color = vec4(1.0, 1.0, 1.0, 1.0);
	vec2 p0 = gl_in[0].gl_Position.xy;
	vec2 p1 = gl_in[1].gl_Position.xy;
	vec2 p2 = gl_in[2].gl_Position.xy;
	vec2 p3 = gl_in[3].gl_Position.xy;

	// calculate tangents and normal vectors
	vec2 p01 = normalize(p1 - p0);
	vec2 p12 = normalize(p2 - p1);
	vec2 p23 = normalize(p3 - p2);

	vec2 t1 = normalize(p01 + p12);
	vec2 t2 = normalize(p12 + p23);

	// calculate miter
	vec2 m1 = vec2(-t1.y, t1.x);
	vec2 m2 = vec2(-t2.y, t2.x);

	vec2 n = vec2(-p12.y, p12.x);
	float l1 = width / dot(m1, n);
	float l2 = width / dot(m2, n);
	
	gl_Position = vec4(m1 * l1 + p1, 0.0, 1.0);
	EmitVertex();

	gl_Position = vec4(m2 * l2 + p2, 0.0, 1.0);
	EmitVertex();

	gl_Position = vec4(-m1 * l1 + p1, 0.0, 1.0);
	EmitVertex();

	gl_Position = vec4(-m2 * l2 + p2, 0.0, 1.0);
	EmitVertex();

	EndPrimitive();
}	
)"
