#include "application.h"

void FF::Application::run() {
	initWindow();
	initVulKan();
	mainLoop();
	cleanUp();
}

void FF::Application::initWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	mWindow = glfwCreateWindow(WIDTH, HEIGHT, "Window", NULL, NULL);

	if (!mWindow) std::cerr << "error:fail init window" << std::endl;

}

void FF::Application::initVulKan(){
	mInstance = Wrapper::Instance::create(true);
	mDevice = Wrapper::Device::create(mInstance);
}

void FF::Application::mainLoop() {
	while (!glfwWindowShouldClose(mWindow)) {
		glfwPollEvents();
	}

}

void FF::Application::cleanUp() {
	mDevice.reset();
	mInstance.reset();
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

