R"(
#version 150

layout(lines) in;
layout(points, max_vertices = 2) out;
in vec2 gxin[];
in float index[];

out vec2 xout;

uniform float N;

const float pi = radians(180.);
void main () {
	float k = index[0];
	float pi_nk = pi / N * k;
	float ak = 0.5 - 0.5 * sin(pi_nk);
	float aik = -0.5 * cos(pi_nk);
	float bk = 0.5 + 0.5 * sin(pi_nk);
	float bik = 0.5 * cos(pi_nk);

	xout = vec2(gxin[0].x * ak - gxin[0].y * aik + gxin[1].x * bk + gxin[1].y * bik, gxin[0].y * ak + gxin[0].x * aik + gxin[1].x * bik - gxin[1].y * bk);
	//xout = vec2(index[0],k);
	EmitVertex();

	pi_nk = pi / N * (N-1.-k);
	ak = 0.5 - 0.5 * sin(pi_nk);
	aik = -0.5 * cos(pi_nk);
	bk = 0.5 + 0.5 * sin(pi_nk);
	bik = 0.5 * cos(pi_nk);

	xout = vec2(gxin[1].x * ak - gxin[1].y * aik + gxin[0].x * bk + gxin[0].y * bik, gxin[1].y * ak + gxin[1].x * aik + gxin[0].x * bik - gxin[0].y * bk);
	//xout = vec2(index[1],(N-1.-k));
	EmitVertex();
}
)"
