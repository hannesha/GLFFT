#include "Cube.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Cube::Cube(){
	init_cube();
	rotate(0.0);
}

void Cube::draw(){
	sh_cube();
	v_cube.bind();
	//b_cube_el.elbind();
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, nullptr);
}

void Cube::init_cube(){
	const char* vert_code = 
	"#version 150\n"
	"in vec3 pos;"
	"out vec4 color;"
	"uniform mat4 proj;"
	"uniform mat4 view;"
	"uniform mat4 model;"
	"void main(){"
	"  color = vec4(1.0, 1.0, 1.0, 1.0);"
	"  gl_Position = proj * view * model * vec4(pos, 1.0);"
	"}";

	GL::Shader vert(vert_code, GL_VERTEX_SHADER);

	const char* frag_code =
	#include "shader/simple.frag"
	;

	GL::Shader frag(frag_code, GL_FRAGMENT_SHADER);

	sh_cube.link({vert, frag});

	sh_cube();

	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	GLint uniView = sh_cube.get_uniform("view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
	GLint uniProj = sh_cube.get_uniform("proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	const float cube_verts[] = {
	//x    y    z
	-0.5, 0.5, 0.5,
	-0.5, 0.5,-0.5,
	 0.5, 0.5,-0.5,
	 0.5, 0.5, 0.5,
	-0.5,-0.5, 0.5,
	-0.5,-0.5,-0.5,
	 0.5,-0.5,-0.5,
	 0.5,-0.5, 0.5
	};

	v_cube.bind();

	b_cube.bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_verts), cube_verts, GL_STATIC_DRAW);

	GLint arg_pos = sh_cube.get_attrib("pos");
	glVertexAttribPointer(arg_pos, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(arg_pos);

	const unsigned short cube_elements[] ={
	0,1, 1,2, 2,3, 3,0,
	4,5, 5,6, 6,7, 7,4,
	0,4, 1,5, 2,6, 3,7
	};

	b_cube_el.bind(GL_ELEMENT_ARRAY_BUFFER);
	// fill dat shit boy
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

	GL::VAO::unbind();
	GL::Buffer::unbind(GL_ELEMENT_ARRAY_BUFFER);
}

void Cube::rotate(const float time){
	sh_cube();

	glm::mat4 model;
	model = glm::rotate(
		model,
		time * glm::radians(180.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	GLint uniModel = sh_cube.get_uniform("model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
}
