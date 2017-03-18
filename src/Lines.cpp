#include "Lines.hpp"

#include <iostream>
#include <vector>

Lines::Lines(){
	init_lines();
}

void Lines::draw(){
	sh_lines();
	v_lines.bind();
	glDrawArrays(GL_LINE_STRIP_ADJACENCY, 1, 7);
}

void Lines::init_lines(){
	const char* vert_code = 
	"#version 150\n"
	"in vec2 pos;"
	"void main(){"
	"  gl_Position = vec4(pos, 1.0, 1.0);"
	"}";

	GL::Shader vert(vert_code, GL_VERTEX_SHADER);
	std::cout << "linking vertex shader:" << std::endl;
	std::cout << glGetError() << std::endl;

	const char* geom_code =
	#include "shader/smooth_lines.geom"
	;

	GL::Shader geom(geom_code, GL_GEOMETRY_SHADER);
	std::cout << "linking geometry shader:" << std::endl;
	std::cout << glGetError() << std::endl;

	const char* frag_code =
	#include "shader/simple.frag"
	;

	GL::Shader frag(frag_code, GL_FRAGMENT_SHADER);
	std::cout << "linking fragment shader:" << std::endl;
	std::cout << glGetError() << std::endl;

	sh_lines.link({vert, geom, frag});
	std::cout << "linking shader:" << std::endl;
	std::cout << glGetError() << std::endl;

	const float line_verts[] = {
	//x    y
	-1.0,-0.5,
	-0.5,-0.5,
	-0.2,-0.5,
	-0.5, 0.0,
	 0.0, 0.5,
	 0.5, 0.0,
	 0.2,-0.5,
	 0.5,-0.5,
	 1.0,-0.5
	};

	v_lines.bind();

	b_lines.bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_verts), line_verts, GL_STATIC_DRAW);
	std::cout << "filling buffer:" << std::endl;
	std::cout << glGetError() << std::endl;

	sh_lines();
	std::cout << "use shader:" << std::endl;
	std::cout << glGetError() << std::endl;

	GLint arg_pos = sh_lines.get_attrib("pos");
	glVertexAttribPointer(arg_pos, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(arg_pos);
	std::cout << "set attribute:" << std::endl;
	std::cout << glGetError() << std::endl;

	GLint arg_width = sh_lines.get_uniform("width");
	glUniform1f(arg_width, 0.1f);
	std::cout << "set uniform:" << std::endl;
	std::cout << glGetError() << std::endl;
}
