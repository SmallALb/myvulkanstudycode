#pragma once

#include "../base.h"
#include "../vulKanWrapper/instance.h"
#include "../vulKanWrapper/device.h"

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



	private:
		GLFWwindow* mWindow { nullptr };
		Wrapper::Instance::Ptr mInstance;
		Wrapper::Device::Ptr mDevice{nullptr};
	};
}