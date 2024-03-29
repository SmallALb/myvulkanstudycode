#pragma once

#include "../base.h"
#include "../vulKanWrapper/instance.h"
#include "../vulKanWrapper/device.h"
#include "../vulKanWrapper/windows.h"
#include "../vulKanWrapper/windowSurface.h"
#include "../vulKanWrapper/swapChain.h"
#include "../vulKanWrapper/shader.h"
#include "../vulKanWrapper/pipline.h"
namespace FF {
	const int WIDTH = 800;
	const int HEIGHT = 600;
	class Application {
	public:
		void run();

		Application() = default;

		~Application() = default;
	private:
		void initWindow();

		void initVulKan();

		void mainLoop();

		void cleanUp();

		void createPipeline();

	private:
		//GLFWwindow* mWindow { nullptr };
		Wrapper::Instance::Ptr mInstance;
		Wrapper::Device::Ptr mDevice{ nullptr };
		Wrapper::Window::Ptr mWindow{ nullptr };
		Wrapper::WindowSurface::Ptr mWindowSurface{nullptr};
		Wrapper::SwapChain::Ptr mSwapChain{ nullptr };
		Wrapper::Pipeline::Ptr mPipeline{nullptr};
	};
}