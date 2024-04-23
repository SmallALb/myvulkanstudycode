#include "windows.h"


namespace FF::Wrapper {

	static void windowsResize(GLFWwindow* window, int width, int height) {
		auto pUserData = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		pUserData->windowresize = 1;
	}

	Window::Window(const int& width, const int& height) {
		mWidth = width;
		mHeight = height;
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		mWindow = glfwCreateWindow(mWidth, mHeight, "vulkan study", NULL, NULL);

		if (!mWindow) std::cerr << "error:fail init window" << std::endl;

		glfwSetWindowUserPointer(mWindow, this);
		glfwSetFramebufferSizeCallback(mWindow, windowsResize);
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