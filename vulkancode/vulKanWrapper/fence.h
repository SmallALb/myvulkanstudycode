#pragma once

#include "device.h"
#include "../base.h"

namespace FF::Wrapper {
	class Fence{
	public:
		using Ptr = std::shared_ptr<Fence>;
		static Ptr create(const Device::Ptr& device, bool signaled = 1) {
			return std::make_shared<Fence>(device, signaled);
		}
		Fence(const Device::Ptr& device, bool signaled = 1);

		~Fence();

		void resetFence();

		void block(uint64_t timeout = UINT64_MAX);

		[[nodiscard]] auto getFence() const { return mFence; }
	private:
		VkFence mFence{ VK_NULL_HANDLE };
		Device::Ptr mDevice{nullptr};

	};

}