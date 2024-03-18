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
	mSwapChain = Wrapper::SwapChain::create(mDevice,mWindow, mWindowSurface);
}

void FF::Application::mainLoop() {
	while (mWindow->shouldClose()) {
		mWindow->pollEvents();
	}

}

void FF::Application::cleanUp() {
	mSwapChain.reset();
	mDevice.reset();
	mWindowSurface.reset();
	mInstance.reset();
	mWindow.reset();
}

