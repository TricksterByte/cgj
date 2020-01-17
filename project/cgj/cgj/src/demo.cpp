#include <iostream>

#include "mesh_manager.h"
#include "texture_manager.h"
#include "shader_manager.h"
#include "scene_manager.h"
#include "keybuffer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glprogram.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

GLProgram program = GLProgram();

#define ERROR_CALLBACK
#ifdef  ERROR_CALLBACK

////////////////////////////////////////////////// ERROR CALLBACK (OpenGL 4.3+)

static const std::string errorSource(GLenum source)
{
	switch (source) {
	case GL_DEBUG_SOURCE_API:				return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "window system";
	case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "shader compiler";
	case GL_DEBUG_SOURCE_THIRD_PARTY:		return "third party";
	case GL_DEBUG_SOURCE_APPLICATION:		return "application";
	case GL_DEBUG_SOURCE_OTHER:				return "other";
	default:								exit(EXIT_FAILURE);
	}
}

static const std::string errorType(GLenum type)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:				return "error";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	return "deprecated behavior";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "undefined behavior";
	case GL_DEBUG_TYPE_PORTABILITY:			return "portability issue";
	case GL_DEBUG_TYPE_PERFORMANCE:			return "performance issue";
	case GL_DEBUG_TYPE_MARKER:				return "stream annotation";
	case GL_DEBUG_TYPE_PUSH_GROUP:			return "push group";
	case GL_DEBUG_TYPE_POP_GROUP:			return "pop group";
	case GL_DEBUG_TYPE_OTHER_ARB:			return "other";
	default:								exit(EXIT_FAILURE);
	}
}

static const std::string errorSeverity(GLenum severity)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:			return "high";
	case GL_DEBUG_SEVERITY_MEDIUM:			return "medium";
	case GL_DEBUG_SEVERITY_LOW:				return "low";
	case GL_DEBUG_SEVERITY_NOTIFICATION:	return "notification";
	default:								exit(EXIT_FAILURE);
	}
}

static void error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar* message, const void* userParam)
{
	std::cerr << "GL ERROR:" << std::endl;
	std::cerr << "  source:     " << errorSource(source) << std::endl;
	std::cerr << "  type:       " << errorType(type) << std::endl;
	std::cerr << "  severity:   " << errorSeverity(severity) << std::endl;
	std::cerr << "  debug call: " << std::endl << message << std::endl;
	std::cerr << "Press <return>.";
	std::cin.ignore();
}

void setupErrorCallback()
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(error, 0);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);

	// suppress "VIDEO memory as the source for buffer memory operations" notifications in the case of using NVIDIA graphics cards
	// suppress "shader being recompiled" notifications in the case of using NVIDIA graphics cards
	std::string vendor = std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
	if (vendor == "NVIDIA Corporation") {
		const GLuint ids_notification[1] = { 131185 };
		const GLuint ids_medium[1] = { 131218 };
		glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER_ARB, GL_DONT_CARE, 1, ids_notification, GL_FALSE);
		glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_PERFORMANCE, GL_DONT_CARE, 1, ids_medium, GL_FALSE);
	}
	// params: source, type, severity, count, ids, enabled
}

#endif // ERROR_CALLBACK

///////////////////////////////////////////////////////////////////// CALLBACKS

void window_close_callback(GLFWwindow* win)
{
	ShaderManager::freeInstance();
	SceneManager::freeInstance();
	MeshManager::freeInstance();
	TextureManager::freeInstance();
	KeyBuffer::freeInstance();
}

void window_size_callback(GLFWwindow* win, int winx, int winy)
{
	glViewport(0, 0, winx, winy);
}

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(win, GLFW_TRUE);
		window_close_callback(win);
	}
	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		program.fireParticleSystem->toggle();
		program.rainParticleSystem->toggle();
		program.map->lightIntensityToggle();
	}

	switch (action) {
	case GLFW_PRESS:
		KeyBuffer::getInstance()->pressKey(key);
		break;
	case GLFW_RELEASE:
		KeyBuffer::getInstance()->releaseKey(key);
		break;
	default:
		break;
	}
}

void mouse_button_callback(GLFWwindow* win, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			if (program.state == MouseState::STOPPED)
				program.state = MouseState::STARTING;
		}
		else if (action == GLFW_RELEASE) {
				program.state = MouseState::STOPPED;
		}
	}
}

void mouse_callback(GLFWwindow* win, double xpos, double ypos)
{
	switch (program.state) {
	case MouseState::STOPPED:
		break;
	case MouseState::STARTING:
		program.lastPosition.x = (float)xpos;
		program.lastPosition.y = (float)ypos;
		program.state = MouseState::ROTATING;
		break;
	case MouseState::ROTATING:
		program.offsetX = (float)(program.lastPosition.x - xpos) * SceneManager::getInstance()->get("scenegraph")->camera->getSensitivity();
		program.offsetY = (float)(program.lastPosition.y - ypos) * SceneManager::getInstance()->get("scenegraph")->camera->getSensitivity();

		program.lastPosition.x = (float)xpos;
		program.lastPosition.y = (float)ypos;
		break;
	}
}

void mouse_scroll_callback(GLFWwindow* win, double xpos, double ypos)
{
	program.yDistance = (float)ypos;
}

///////////////////////////////////////////////////////////////////////// SETUP

void glfw_error_callback(int error, const char* description)
{
	std::cerr << "GLFW Error: " << description << std::endl;
}

GLFWwindow* setupWindow(int winx, int winy, const char* title,
	int is_fullscreen, int is_vsync)
{
	GLFWmonitor* monitor = is_fullscreen ? glfwGetPrimaryMonitor() : 0;
	GLFWwindow* win = glfwCreateWindow(winx, winy, title, monitor, 0);
	if (!win)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(win);
	glfwSwapInterval(is_vsync);
	return win;
}

void setupCallbacks(GLFWwindow* win)
{
	glfwSetWindowCloseCallback(win, window_close_callback);
	glfwSetWindowSizeCallback(win, window_size_callback);
	glfwSetKeyCallback(win, key_callback);
	glfwSetCursorPosCallback(win, mouse_callback);
	glfwSetMouseButtonCallback(win, mouse_button_callback);
	glfwSetScrollCallback(win, mouse_scroll_callback);
}

GLFWwindow* setupGLFW(int gl_major, int gl_minor,
	int winx, int winy, const char* title, int is_fullscreen, int is_vsync)
{
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	GLFWwindow* win = setupWindow(winx, winy, title, is_fullscreen, is_vsync);
	setupCallbacks(win);

#if _DEBUG
	std::cout << "GLFW " << glfwGetVersionString() << std::endl;
#endif
	return win;
}

void setupGLEW()
{
	glewExperimental = GL_TRUE;
	// Allow extension entry points to be loaded even if the extension isn't 
	// present in the driver's extensions string.
	GLenum result = glewInit();
	if (result != GLEW_OK)
	{
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
	// You might get GL_INVALID_ENUM when loading GLEW.
}

void checkOpenGLInfo()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;
}

void setupOpenGL(int winx, int winy)
{
#if _DEBUG
	checkOpenGLInfo();
#endif
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glViewport(0, 0, winx, winy);
}

GLFWwindow* setup(int major, int minor,
	int winx, int winy, const char* title, int is_fullscreen, int is_vsync)
{
	GLFWwindow* win =
		setupGLFW(major, minor, winx, winy, title, is_fullscreen, is_vsync);
	setupGLEW();
	setupOpenGL(winx, winy);
#ifdef ERROR_CALLBACK
	setupErrorCallback();
#endif

	program.createShaders();
	program.createMeshes();
	program.createTextures();
	program.createSceneGraph();

	return win;
}

////////////////////////////////////////////////////////////////////////// RUN

void display(GLFWwindow* win, float elapsed_sec)
{
	bool offset_changed = false;
	if (KeyBuffer::getInstance()->isKeyPressed(GLFW_KEY_Q)) {
		program.Offset += program.OffsetDelta;
		offset_changed = true;
	}
	if (KeyBuffer::getInstance()->isKeyPressed(GLFW_KEY_A)) {
		program.Offset -= program.OffsetDelta;
		offset_changed = true;
	}
	if (offset_changed) {
		program.Offset = program.Offset < 0.0f ? 0.0f : program.Offset;

		ShaderProgram* shader = ShaderManager::getInstance()->get("silhouette");
		shader->bind();
		glUniform1f(shader->uniforms["Offset"].index, program.Offset);
		shader->unbind();
	}
	
	program.update(elapsed_sec);
	program.setViewProjectionMatrix();
	program.drawSceneGraph();
}

void run(GLFWwindow* win)
{
	double last_time = glfwGetTime();
	while (!glfwWindowShouldClose(win))
	{
		double time = glfwGetTime();
		program.elapsed_time = (float)(time - last_time);
		last_time = time;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		display(win, program.elapsed_time);
		glfwSwapBuffers(win);
		glfwPollEvents();
	}
	glfwDestroyWindow(win);
	glfwTerminate();
}

////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[])
{
	int gl_major = 4, gl_minor = 3;
	int is_fullscreen = 0;
	int is_vsync = 1;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	GLFWwindow* win = setup(gl_major, gl_minor,
		1080, 720, "Nobody's Rainy Tent", is_fullscreen, is_vsync);

	run(win);
	exit(EXIT_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////// END