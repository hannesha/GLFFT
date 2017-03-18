#pragma once

#include "Program.hpp"

class Cube {
	public:
		Cube();
		~Cube(){};

		void draw();
		void rotate(const float);
	private:
		Program sh_cube;
		GL::VAO v_cube;
		GL::Buffer b_cube, b_cube_el;

		void init_cube();
};
