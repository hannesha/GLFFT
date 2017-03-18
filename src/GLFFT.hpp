#pragma once

#include "Program.hpp"
#include <vector>
#include <memory>

namespace GL {
	// VBO RAII wrapper
	class Buffers{
		public:
			inline Buffers(const unsigned n) : ids(n) { glGenBuffers(n, ids.data()); };
			inline ~Buffers() { glDeleteBuffers(ids.size(), ids.data()); };
			// disable copying
			Buffers(const Buffers&) = delete;

			inline void bind(const unsigned i) { glBindBuffer(GL_ARRAY_BUFFER, ids[i]); };
			inline void bind(const unsigned i, GLenum target) { glBindBuffer(target, ids[i]); };
			inline void operator()(const unsigned i){ bind(i); };
			std::vector<GLuint> ids;
	};
}

class GLFFT {
	public:
		GLFFT(const size_t);
		~GLFFT(){};

		void calculate();
		void calculate_r2c();
		void fill(const std::vector<float>&);
		void get(std::vector<float>&);
		void get_r2c(std::vector<float>&);

		template<typename T> static void dumpvect(const T& x){
			std::cout << "vector:" << std::endl;
			for(auto e : x){
				std::cout << e << std::endl;
			}
		}

		template<typename T> static void swap(T& x, const size_t i1, const size_t i2){
			typename T::value_type tmp = x[i1];
			x[i1] = x[i2];
			x[i2] = tmp;
		}

		template<typename T> static void perm(T& x, const size_t n, const size_t s){
			for(unsigned i = 0; i < n/2; i++){
				swap(x, i+s, 2*i+s);
			}
			if(n > 2){
				perm(x, n/2, s);
			}
		}

		template<typename T> static void permutate(T& x, const size_t n){
			for(unsigned i = 0; i < x.size()/n; i++){
				perm(x, n, n * i);	
			}
		}

		template<typename T> static void permutate_r2c(T& x, const T& y){
			for(unsigned i = 0; i < y.size()/2; i++){
				x[2*i] = y[i];
				x[2*i+1] = y[y.size()-1 -i];
			}
		}
	private:
		size_t size;
		GL::Program sh_dft, sh_pass, sh_r2c;
		GL::VAO v_dft[2], v_entry;
		GL::Buffer b_data, b_dft[2], b_r2c_el;
		GL::Buffers b_dft_el; // bit reversal element buffers
		
		void fill_b_el();
		void fill_b_r2c();
		void init_pass();
		void init_dfts();
		void init_r2c();
};
