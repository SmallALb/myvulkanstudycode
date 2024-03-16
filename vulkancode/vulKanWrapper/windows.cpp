#include "windows.h"


namespace FF::Wrapper {

	Window::Window(const int& width, const int& height) {
		mWidth = width;
		mHeight = height;
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		mWindow = glfwCreateWindow(mWidth, mHeight, "vulkan study", NULL, NULL);

		if (!mWindow) std::cerr << "error:fail init window" << std::endl;

	}

	Window::~Window() {
		glfwDestroyWindow(mWindow);
		glfwTerminate();
	}
	bool Window::shouldClose() {
		return !glfwWindowShouldClose(mWindow);
	}

	void Window::pollEvents() {
		glfwPollEvents();
	}

}