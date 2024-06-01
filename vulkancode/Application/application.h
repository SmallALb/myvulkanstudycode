#pragma once

#include "../base.h"
#include "../vulKanWrapper/instance.h"
#include "../vulKanWrapper/device.h"
#include "../vulKanWrapper/windows.h"
#include "../vulKanWrapper/windowSurface.h"
#include "../vulKanWrapper/swapChain.h"
#include "../vulKanWrapper/shader.h"
#include "../vulKanWrapper/pipeline.h"
#include "../vulKanWrapper/renderPass.h"
#include "../vulKanWrapper/commandPool.h"
#include "../vulKanWrapper/commandBuffer.h"
#include "../vulKanWrapper/semaphore.h"
#include "../vulKanWrapper/fence.h"
#include "../vulKanWrapper/buffer.h"
#include "../vulKanWrapper/descriptorLayout.h"
#include "../vulKanWrapper/descriptorPool.h"
#include "../vulKanWrapper/description.h"
#include "../vulKanWrapper/descriptorSet.h"
#include "../uniformManager.h"
#include "../model.h"

namespace FF {



	class Application {
	public:
		void run();

		Application() = default;

		~Application() = default;
	private:
		//初始化窗口
		void initWindow();
		
		//初始化vulkan引擎
		void initVulKan();
		
		//主要的运行函数
		void mainLoop();

		void cleanUp();

		void createPipeline();

		void createRenderPass();

		void createCommandBuffers();

		void createSyncObjects();

		void cleanupSwapChain();

		//void createDescriptorSetLayout();

		void recreateSwapChain();

		void render();
	private:
		unsigned int mWidth = 800;
		unsigned int mHeight = 600;
	private:
		//GLFWwindow* mWindow { nullptr };
		int mCurrentFrame{ 0 };
		Wrapper::Instance::Ptr mInstance;
		Wrapper::Device::Ptr mDevice{ nullptr };
		Wrapper::Window::Ptr mWindow{ nullptr };
		Wrapper::WindowSurface::Ptr mWindowSurface{nullptr};
		Wrapper::SwapChain::Ptr mSwapChain{ nullptr };
		Wrapper::Pipeline::Ptr mPipeline{nullptr};
		Wrapper::RenderPass::Ptr mRenderPass{nullptr};
		Wrapper::CommandPool::Ptr mCommandPool{ nullptr };
		std::vector<Wrapper::CommandBuffer::Ptr> mCommandBuffers{};
		std::vector<Wrapper::Semaphore::Ptr> mImageAvailableSemaphores{};
		std::vector<Wrapper::Semaphore::Ptr> mRenderFinishedSemaphores{};
		std::vector<Wrapper::Fence::Ptr> mFences{};

		UniformManager::Ptr mUniformManager{nullptr};

		Model::Ptr mModel{ nullptr };
		VPMatrices mVPMatrices;
	};
}