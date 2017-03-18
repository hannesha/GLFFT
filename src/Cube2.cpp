#include "Cube2.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Cube2::Cube2(){
	init_cube();
	rotate(0.0);
}

void Cube2::draw(){
	v_cube.bind();
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glCullFace(GL_FRONT);
	sh_cube();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);

	glCullFace(GL_BACK);
	sh_ccube();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}

void Cube2::init_cube(){
	const char* vert_code = 
	"#version 150\n"
	"in vec3 pos;"
	"out vec4 color;"
	"uniform mat4 proj;"
	"uniform mat4 view;"
	"uniform mat4 model;"
	"void main(){"
	"  color = vec4(pos + vec3(0.5), 1.0);"
	"  gl_Position = proj * view * model * vec4(pos, 1.0);"
	"}";

	const char* cvert_code = 
	"#version 150\n"
	"in vec3 pos;"
	"out vec4 color;"
	"uniform mat4 proj;"
	"uniform mat4 view;"
	"uniform mat4 model;"
	"void main(){"
	"  color = vec4(vec3(1.0), 1.0);"
	"  gl_Position = proj * view * model * vec4(pos, 1.0);"
	"}";

	GL::Shader vert(vert_code, GL_VERTEX_SHADER);
	GL::Shader cvert(cvert_code, GL_VERTEX_SHADER);

	const char* frag_code =
	#include "shader/simple.frag"
	;

	GL::Shader frag(frag_code, GL_FRAGMENT_SHADER);

	sh_cube.link({vert, frag});
	sh_ccube.link({cvert, frag});

	sh_cube();

	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 1.0f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	GLint uniView = sh_cube.get_uniform("view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
	GLint uniProj = sh_cube.get_uniform("proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	sh_ccube();
	uniView = sh_ccube.get_uniform("view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	uniProj = sh_ccube.get_uniform("proj");
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
	/*const float cube_verts[] = {
	-0.433,-0.25, 0.25,
	 0.0,-0.25,-0.5,
	 0.433,-0.25, 0.25,
	 0.0, 0.25, 0.0
	};*/

	v_cube.bind();

	b_cube.bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_verts), cube_verts, GL_STATIC_DRAW);

	GLint arg_pos = sh_cube.get_attrib("pos");
	glVertexAttribPointer(arg_pos, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(arg_pos);

	const unsigned short cube_elements[] ={
	0,1,2, 0,2,3,
	3,4,0, 3,7,4,
	4,6,5, 4,7,6,
	2,7,3, 2,6,7,
	1,4,5, 1,0,4,
	2,5,6, 2,1,5
	};
	/*const unsigned short cube_elements[] ={
	0,3,2, 2,3,1, 1,3,0, 0,2,1
	};*/

	b_cube_el.bind(GL_ELEMENT_ARRAY_BUFFER);
	// fill dat shit boy
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

	GL::VAO::unbind();
	GL::Buffer::unbind(GL_ELEMENT_ARRAY_BUFFER);
}

void Cube2::rotate(const float time){

	glm::mat4 model;
	model = glm::rotate(
		model,
		time * glm::radians(180.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	sh_ccube();
	GLint uniModel = sh_ccube.get_uniform("model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	model = glm::scale(model, glm::vec3(0.9f));

	sh_cube();
	uniModel = sh_cube.get_uniform("model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
}
