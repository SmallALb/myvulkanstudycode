#pragma once

#include "../base.h"
#include "instance.h"
#include "windowSurface.h"
namespace FF::Wrapper {
	const std::vector<const char*> deviceRequiredExtenstions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	class Device {
	public:
		using Ptr = std::shared_ptr<Device>;

		static Ptr create(Instance::Ptr instance, WindowSurface::Ptr Surface) { return std::make_shared<Device>(instance,Surface); }

		Device(Instance::Ptr instance, WindowSurface::Ptr Surface);

		~Device();

		void pickPhysicalDevice();

		int rateDevice(VkPhysicalDevice device);

		bool isDeviceSuitable(VkPhysicalDevice device);

		void initQueueFamiles(VkPhysicalDevice device);

		void createLogicalDevice();
		
		bool isQueueFamilyComplete();

		 [[nodiscard]] auto getDevice() const { return mDevice; }

		 [[nodiscard]] auto getPhysicalDevice() const { return mPhysicalDevice; }

		 [[nodiscard]] auto getGraphicQueueFamily() const { return mGraphicQueueFamily; }

		 [[nodiscard]] auto getPresentQueueFamily() const { return mPresentQueueFamily; }

		 [[nodiscard]] auto getGraphicQueue() const { return mGraphicQueue; }

		 [[nodiscard]] auto getPresentQueue() const { return mPresentQueue; }
	private:
		VkPhysicalDevice mPhysicalDevice{VK_NULL_HANDLE};
		Instance::Ptr mInstance{ nullptr };
		std::optional<uint32_t> mGraphicQueueFamily;
		VkQueue mGraphicQueue{ VK_NULL_HANDLE };
		WindowSurface::Ptr mSurface{nullptr};
		std::optional<uint32_t> mPresentQueueFamily;
		VkQueue mPresentQueue{ VK_NULL_HANDLE };

		VkDevice mDevice{ VK_NULL_HANDLE };

	};

}