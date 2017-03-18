#pragma once

#include "Program.hpp"

class Lines {
	public:
		Lines();
		~Lines(){};

		void draw();
	private:
		Program sh_lines;
		GL::VAO v_lines;
		GL::Buffer b_lines;

		void init_lines();
};
