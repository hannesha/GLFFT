#pragma once

#include "Program.hpp"

class Cube2 {
	public:
		Cube2();
		~Cube2(){};

		void draw();
		void rotate(const float);
	private:
		Program sh_cube, sh_ccube;
		GL::VAO v_cube;
		GL::Buffer b_cube, b_cube_el;

		void init_cube();
};
