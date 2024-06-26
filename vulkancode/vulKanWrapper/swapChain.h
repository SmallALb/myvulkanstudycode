#pragma once

#include "../base.h"
#include "device.h"
#include "windows.h"
#include "windowSurface.h"
#include "renderPass.h"

namespace FF::Wrapper {

	struct SwapChainSupportInfo {
		VkSurfaceCapabilitiesKHR mCapabilities;
		std::vector<VkSurfaceFormatKHR> mFormats;
		std::vector<VkPresentModeKHR> mPresentModes;
	};

	class SwapChain {
	public:
		using Ptr = std::shared_ptr<SwapChain>;

		static Ptr create(const Device::Ptr& device, const Window::Ptr& window, const WindowSurface::Ptr& surface) {
			return std::make_shared<SwapChain>(device, window, surface);
		}

		SwapChain(const Device::Ptr &device, const Window::Ptr &window, const WindowSurface::Ptr &surface);

		~SwapChain();

		SwapChainSupportInfo querySwapChainSupportInfo();

		VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

		VkPresentModeKHR chooseSurfacePresentMode(const std::vector<VkPresentModeKHR> &availablePresenstModes);

		VkExtent2D chooseExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		[[nodiscard]] auto getFormat() const { return mSwapChainFormat; }

		[[nodiscard]] auto getImageCount() const { return mImageCount; }

		[[nodiscard]] auto getSwapChain() const { return mSwapChain; }

		[[nodiscard]] auto getFrameBuffer(const int index) const { return mSwapChainFrameBuffers[index]; }

		[[nodiscard]] auto getExtent() const { return mSwapChainExtent; }

		void createFrameBuffers(const RenderPass::Ptr &renderPass);
	private:
		VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels=0);

	private:
		VkSwapchainKHR mSwapChain{VK_NULL_HANDLE};

		VkFormat mSwapChainFormat;
		VkExtent2D mSwapChainExtent;
		uint32_t mImageCount{ 0 };

		std::vector<VkImage> mSwapChainImages{};
		std::vector<VkImageView> mSwapChainImageViews{};

		std::vector<VkFramebuffer> mSwapChainFrameBuffers{};

		Device::Ptr mDevice{nullptr};
		Window::Ptr mWindow{nullptr};
		WindowSurface::Ptr mSurface{nullptr};
	};

}

