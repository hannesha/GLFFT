#pragma once

#define GL_GLEXT_PROTOTYPES
#include <GL/glcorearb.h>
#include <iostream>

namespace GL {
	// VBO RAII wrapper
	class Buffer{
		public:
			inline Buffer() { glGenBuffers(1, &id); };
			inline ~Buffer() { glDeleteBuffers(1, &id); };
			// disable copying
			Buffer(const Buffer&) = delete;

			inline void bind() { glBindBuffer(GL_ARRAY_BUFFER, id); };
			inline void bind(GLenum target) { glBindBuffer(target, id); };
			static inline void unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); };
			static inline void unbind(GLenum target) { glBindBuffer(target, 0); };
			inline void operator()(){ bind(); };
			inline void tfbind() { glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, id); };
			GLuint id;
	};

	// VAO RAII wrapper
	class VAO{
		public:
			inline VAO() { glGenVertexArrays(1, &id); };
			inline ~VAO() { glDeleteVertexArrays(1, &id); };
			// disable copying
			VAO(const VAO&) = delete;

			inline void bind() { glBindVertexArray(id); };
			static inline void unbind() { glBindVertexArray(0); };
			inline void operator()(){ bind(); };
			GLuint id;
	};

	// Shader RAII wrapper
	class Shader {
		public:
			inline Shader(const char* code, GLuint type){
				id = glCreateShader(type);
				glShaderSource(id, 1, &code, nullptr);
				glCompileShader(id);
			};
			inline ~Shader(){ glDeleteShader(id); };
			// disable copying
			Shader(const Shader&) = delete;

			GLuint id;
	};

	// Framebuffer RAII wrapper
	class FBO {
		public:
			inline FBO(){ glGenFramebuffers(1, &id); };
			inline ~FBO(){ glDeleteFramebuffers(1, &id); };
			FBO(const FBO&) = delete;

			inline void bind(){ glBindFramebuffer(GL_FRAMEBUFFER, id); };
			inline void operator()(){ bind(); };
			GLuint id;
	};

	// Texture RAII wrapper
	class Texture {
		public:
			inline Texture(){ glGenTextures(1, &id); };
			inline ~Texture(){ glDeleteTextures(1, &id); };
			Texture(const Texture&) = delete;

			inline void bind(GLenum type){ glBindTexture(type, id); };
			inline void operator()(GLenum type){ bind(type); };
			GLuint id;
	};
}
