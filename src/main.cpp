#include "main.hpp"

#include <memory>
#include <stdexcept>
#include <chrono>
#include <cmath>

// make_unique template for backwards compatibility
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

class glfw_error : public std::runtime_error {
public :
	glfw_error(const char* what_arg) : std::runtime_error(what_arg){};
};

int main(){
	try{
		// init GLFW
		if(!glfwInit()) throw std::runtime_error("GLFW init failed!");

		glfwWindowHint(GLFW_SAMPLES, 8);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef EGL
		glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
#endif

		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); //invisible window

		std::stringstream title;
		title << "TEST FFT" ;

		// create GLFW window
		GLFWwindow* window;
		window = glfwCreateWindow( 800, 600, title.str().c_str(), NULL, NULL);
		if( window == NULL ){
			glfwTerminate();
			throw glfw_error("Failed to create GLFW Window!");
		}

		glfwMakeContextCurrent(window);

		// init GLEW
		/*if(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) != GL_TRUE){
			glfwTerminate();
			throw glfw_error("GLEW init failed!");
		}*/
		//std::cout << glGetError() << std::endl;

		// set clear color to black
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		unsigned size = 4096;
		std::cout << "FFT size: " << size << std::endl;
		
		GLFFT fft(size);
		std::vector<float> test(size * 2, 0);
		test[0] = 1;
		test[1] = 1;
		/*test[4] = 1;
		test[6] = 1;
		test[8] = 1;
		test[10] = 1;
		test[12] = 1;
		test[14] = 1;*/

		//GLFFT::dumpvect(test);
		FFT fftw(size*2);
		fftw.fill(test);
		fftw.calculate();
		//fftw.calculate();
		std::cout << "FFTW:" << std::endl;
		for (unsigned i = 0; i < 4; i++){
			std::cout << fftw.output[i][0] << std::endl;
			std::cout << fftw.output[i][1] << std::endl;
		}
		fft.fill(test);
		fft.calculate_r2c();
		fft.get_r2c(test);
		std::cout << "GL:" << std::endl;
		for (unsigned i = 0; i < 8; i++){
			std::cout << test[i] << std::endl;
		}
		//GLFFT::dumpvect(test);
		/*float avgtime_gl = 0;
		float avgtime_fftw = 0;
		for(unsigned i = 0; i < 10; i++){
			std::chrono::time_point<std::chrono::steady_clock> t_start = std::chrono::steady_clock::now();
			fftw.calculate();
			std::chrono::time_point<std::chrono::steady_clock> t_stop = std::chrono::steady_clock::now();
			float time = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(t_stop - t_start).count();
			avgtime_fftw += time;
			std::cout << "fftw time:" << time << std::endl;

			t_start = std::chrono::steady_clock::now();
			fft.calculate_r2c();
			t_stop = std::chrono::steady_clock::now();
			time = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(t_stop - t_start).count();
			avgtime_gl += time;
			std::cout << "gl time:" << time << std::endl;
		}	
		//float mflops = (5. * (float)size * std::log2(size)) / (avgtime/10.);
		std::cout << "fftw avgtime:" << avgtime_fftw/10 << std::endl;//" mflops:" << mflops << std::endl;
		std::cout << "gl avgtime:" << avgtime_gl/10 << std::endl;*/
		

		std::cout << "init done:" << std::endl;
		std::cout << glGetError() << std::endl;

		/*std::chrono::time_point<std::chrono::steady_clock> t_start = std::chrono::steady_clock::now();
		do{
			std::chrono::time_point<std::chrono::steady_clock> t_fps = std::chrono::steady_clock::now() + std::chrono::microseconds(1000000 / 60 -100);


			float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_fps - t_start).count();
			glfwSwapBuffers(window);
			glfwPollEvents();

			// wait for fps timer
			std::this_thread::sleep_until(t_fps);
		} // Wait until window is closed
		while(glfwWindowShouldClose(window) == 0);*/

		glfwTerminate();
	}catch(std::runtime_error& e){
		// print error message and terminate with error code 1
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
