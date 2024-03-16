#include "application.h"

void FF::Application::run() {
	initWindow();
	initVulKan();
	mainLoop();
	cleanUp();
}

void FF::Application::initWindow() {
	mWindow = Wrapper::Window::create(WIDTH, HEIGHT);
}

void FF::Application::initVulKan(){
	mInstance = Wrapper::Instance::create(true);
	mWindowSurface = Wrapper::WindowSurface::create(mInstance, mWindow);
	mDevice = Wrapper::Device::create(mInstance, mWindowSurface);
}

void FF::Application::mainLoop() {
	while (mWindow->shouldClose()) {
		mWindow->pollEvents();
	}

}

void FF::Application::cleanUp() {
	mDevice.reset();
	mWindowSurface.reset();
	mInstance.reset();
	mWindow.reset();
}

