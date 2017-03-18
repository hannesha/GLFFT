R"(
#version 150

layout(lines) in;
layout(points, max_vertices = 2) out;
in vec2 gxin[];
in float index[];

out vec2 xout;

uniform float N;

vec2 cmult(vec2 a, vec2 b){
	return vec2(a.x*b.x-a.y*b.y, a.x*b.y-a.y*b.x);
}

const float tau = radians(360.);
void main () {
	float k = mod(index[0], N * 0.5);
	float twiddle = tau * k / N;
	vec2 vtwiddle = vec2(cos(twiddle),-sin(twiddle));
	vec2 txx1 = cmult(vtwiddle, gxin[1]);

	xout = gxin[0] + txx1;
	//xout = vec2(index[0],k);
	EmitVertex();

	xout = gxin[0] - txx1;
	//xout = vec2(index[1],k);
	EmitVertex();
}
)"
