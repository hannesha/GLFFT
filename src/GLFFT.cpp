#include "GLFFT.hpp"

#include <iostream>
#include <cmath>

GLFFT::GLFFT(const size_t size) : 
	size(std::pow(2, std::ceil(std::log2(size)))), //round up to the next base of 2
	b_dft_el(std::ceil(std::log2(size)) -1) // create permutation buffers
{
	//std::cout << (unsigned) std::log2(size)-1 << std::endl;
	fill_b_el();
	fill_b_r2c();

	init_pass();
	init_dfts();
	init_r2c();
}

void GLFFT::fill(const std::vector<float>& data){
	b_data.bind();
	// zero pad
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * size, nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * data.size(), data.data());
	GL::Buffer::unbind();
}

// complex data fft
void GLFFT::calculate(){
	glEnable(GL_RASTERIZER_DISCARD);
	sh_dft();
	GLuint a_N = sh_dft.get_uniform("N");

	v_entry();
	glUniform1f(sh_dft.get_uniform("scale"), 1.);
	glUniform1f(a_N, 2.);

	b_dft[0].tfbind();
	b_dft_el.bind(0, GL_ELEMENT_ARRAY_BUFFER);

	// N=2 dft
	glBeginTransformFeedback(GL_POINTS);
	glDrawElements(GL_LINES, size, GL_UNSIGNED_SHORT, nullptr);
	glEndTransformFeedback();

	GL::Buffer::unbind(GL_TRANSFORM_FEEDBACK_BUFFER);

	unsigned logn = std::log2(size);
	for(unsigned i = 1; i < logn; i++){
		/*std::vector<float> result(2*size);
		b_dft[(i-1)%2]();
		glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * result.size(), result.data());
		dumpvect(result);*/

		glUniform1f(a_N, (float)(2<<i));
		v_dft[i%2]();
		b_dft[i%2].tfbind();
		b_dft_el.bind(i%(logn-1), GL_ELEMENT_ARRAY_BUFFER);

		glBeginTransformFeedback(GL_POINTS);
		glDrawElements(GL_LINES, size, GL_UNSIGNED_SHORT, nullptr);
		glEndTransformFeedback();

		GL::Buffer::unbind(GL_TRANSFORM_FEEDBACK_BUFFER);
	}

	v_dft[(logn)%2]();
	b_dft[(logn)%2].tfbind();
	b_dft_el.bind(0, GL_ELEMENT_ARRAY_BUFFER);

	sh_pass();
	glUniform1f(sh_pass.get_uniform("scale"), 1./std::sqrt(size));
	glBeginTransformFeedback(GL_POINTS);
	glDrawElements(GL_POINTS, size, GL_UNSIGNED_SHORT, nullptr);
	glEndTransformFeedback();

	GL::Buffer::unbind(GL_TRANSFORM_FEEDBACK_BUFFER);
	GL::VAO::unbind();
	glDisable(GL_RASTERIZER_DISCARD);

	//dumpvect(result);
	//std::cout << result.size() << std::endl;
}

// half sized, real data fft
void GLFFT::calculate_r2c(){
	glEnable(GL_RASTERIZER_DISCARD);
	sh_dft();
	GLuint a_N = sh_dft.get_uniform("N");

	v_entry();
	glUniform1f(sh_dft.get_uniform("scale"), 1.);
	glUniform1f(a_N, 2.);

	b_dft[0].tfbind();
	b_dft_el.bind(0, GL_ELEMENT_ARRAY_BUFFER);

	// N=2 dft
	glBeginTransformFeedback(GL_POINTS);
	glDrawElements(GL_LINES, size, GL_UNSIGNED_SHORT, nullptr);
	glEndTransformFeedback();

	GL::Buffer::unbind(GL_TRANSFORM_FEEDBACK_BUFFER);

	unsigned logn = std::log2(size);
	for(unsigned i = 1; i < logn; i++){
		/*std::vector<float> result(2*size);
		b_dft[(i-1)%2]();
		glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * result.size(), result.data());
		dumpvect(result);*/

		glUniform1f(a_N, (float)(2<<i));
		v_dft[i%2]();
		b_dft[i%2].tfbind();
		b_dft_el.bind(i%(logn-1), GL_ELEMENT_ARRAY_BUFFER);

		glBeginTransformFeedback(GL_POINTS);
		glDrawElements(GL_LINES, size, GL_UNSIGNED_SHORT, nullptr);
		glEndTransformFeedback();

		GL::Buffer::unbind(GL_TRANSFORM_FEEDBACK_BUFFER);
	}

	v_dft[(logn)%2]();
	b_dft[(logn)%2].tfbind();
	b_r2c_el.bind(GL_ELEMENT_ARRAY_BUFFER);

	sh_r2c();
	glUniform1f(sh_r2c.get_uniform("scale"), 1.);
	glUniform1f(sh_r2c.get_uniform("N"), (float)size);
	glBeginTransformFeedback(GL_POINTS);
	glDrawElements(GL_LINES, size, GL_UNSIGNED_SHORT, nullptr);
	glEndTransformFeedback();

	GL::Buffer::unbind(GL_TRANSFORM_FEEDBACK_BUFFER);

	v_dft[(logn+1)%2]();
	b_dft[(logn+1)%2].tfbind();
	b_r2c_el.bind(GL_ELEMENT_ARRAY_BUFFER);

	sh_pass();
	glUniform1f(sh_pass.get_uniform("scale"), 1.);
	//glUniform1f(sh_pass.get_uniform("scale"), 1.);
	glBeginTransformFeedback(GL_POINTS);
	glDrawElements(GL_POINTS, size, GL_UNSIGNED_SHORT, nullptr);
	glEndTransformFeedback();

	GL::Buffer::unbind(GL_TRANSFORM_FEEDBACK_BUFFER);
	GL::VAO::unbind();
	glDisable(GL_RASTERIZER_DISCARD);

	//dumpvect(result);
	//std::cout << result.size() << std::endl;
}

void GLFFT::get(std::vector<float>& data){
	unsigned logn = std::log2(size);
	b_dft[logn%2]();
	size_t length = std::min(data.size()/2, size);
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * length * 2, data.data());
	GL::Buffer::unbind();
}

void GLFFT::get_r2c(std::vector<float>& data){
	unsigned logn = std::log2(size) +1;
	b_dft[logn%2]();
	size_t length = std::min(data.size()/2, size);
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * length * 2, data.data());
	GL::Buffer::unbind();
}

void debug(GL::Shader& sh){
	GLint isCompiled = 0;
	glGetShaderiv(sh.id, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(sh.id, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(sh.id, maxLength, &maxLength, &errorLog[0]);
		for(auto e : errorLog){
			std::cout << e;
		}
		std::cout << std::endl;
	}
}

void GLFFT::init_pass(){
	const char* vert_code = 
	#include "shader/pass.vert"
	;

	GL::Shader vert(vert_code, GL_VERTEX_SHADER);
	debug(vert);

	const char* varyings[1] = {"gxin"};
	sh_pass.link_TF(1, varyings, {vert});

	v_entry.bind();

	b_data.bind();
	//glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	//b_cube_el.bind(GL_ELEMENT_ARRAY_BUFFER);
	// fill dat shit boy
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

	GL::VAO::unbind();
	GL::Buffer::unbind();
}

void GLFFT::init_dfts(){
	const char* vert_code = 
	#include "shader/pass.vert"
	;

	GL::Shader vert(vert_code, GL_VERTEX_SHADER);

	const char* geom_code = 
	#include "shader/dft.geom"
	;

	GL::Shader geom(geom_code, GL_GEOMETRY_SHADER);
	debug(geom);

	const char* varyings[1] = {"xout"};
	sh_dft.link_TF(1, varyings, {vert, geom});

	for(unsigned i = 0; i<2; i++){
		v_dft[i].bind();
		b_dft[!i].bind();
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * size, 0, GL_STREAM_READ);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(0);

		GL::VAO::unbind();
		GL::Buffer::unbind();
	}
}

void GLFFT::init_r2c(){
	const char* vert_code = 
	#include "shader/pass.vert"
	;

	GL::Shader vert(vert_code, GL_VERTEX_SHADER);

	const char* geom_code = 
	#include "shader/r2c.geom"
	;

	GL::Shader geom(geom_code, GL_GEOMETRY_SHADER);
	debug(geom);

	const char* varyings[1] = {"xout"};
	sh_r2c.link_TF(1, varyings, {vert, geom});
}

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// initialize permutation buffers
void GLFFT::fill_b_el(){
	std::vector<GLushort> ind(size);

	for(unsigned i = 0; i < ind.size(); i++){
		ind[i] = i;
	}
	unsigned logn = std::log2(size) - 1;
	for(unsigned i = 0; i < logn; i++){
		std::vector<GLushort> tmp(ind);
		permutate(tmp, 2<<(logn-i));
		//std::cout << (2<<(logn-1-i)) << std::endl;
		// create new element buffer
		//dumpvect(tmp);
		b_dft_el.bind(i);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLushort) * tmp.size(), tmp.data(), GL_STATIC_DRAW);
	}
	GL::Buffer::unbind();
}

// initalize r2c permutation buffers
void GLFFT::fill_b_r2c(){
	//std::cout << b_r2c_el.ids.size() << std::endl;
	std::vector<GLushort> ind(size);

	for(unsigned i = 0; i < ind.size(); i++){
		ind[i] = i;
	}

	std::vector<GLushort> tmp(size);
//	permutate_r2c(tmp, ind);
	//std::cout << (2<<(logn-1-i)) << std::endl;
	// create new element buffer
	//dumpvect(tmp);
//	b_r2c_el.bind(0);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(GLushort) * tmp.size(), tmp.data(), GL_STATIC_DRAW);

	permutate(ind, size);
	permutate_r2c(tmp, ind);
	//dumpvect(tmp);
	b_r2c_el.bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLushort) * tmp.size(), tmp.data(), GL_STATIC_DRAW);

	GL::Buffer::unbind();
}
