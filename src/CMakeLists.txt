find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(Threads REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(EGL QUIET egl)
pkg_search_module(FFTW3 REQUIRED fftw3f libfftw3f)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})

if(EGL_FOUND)
	Message("EGL found. Using EGL context creation.")
	add_definitions(-DEGL)
endif(EGL_FOUND)

add_executable(main main.cpp Program.cpp GLFFT.cpp FFT.cpp)

target_link_libraries(main ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLM_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${FFTW3_LIBRARIES})
