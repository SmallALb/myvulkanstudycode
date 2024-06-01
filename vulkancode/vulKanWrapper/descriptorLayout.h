#pragma once
#include "device.h"
#include "../base.h"
#include "description.h"

namespace FF::Wrapper {


	class DescriptorSetLayout {
	public:
		using Ptr = std::shared_ptr<DescriptorSetLayout>;
		static Ptr create(Device::Ptr device) { return std::make_shared<DescriptorSetLayout>(device); }

		DescriptorSetLayout(Device::Ptr device);
		
		~DescriptorSetLayout();

		void bulid(const std::vector<UniformParamenter::Ptr>& params);

		[[nodiscard]] auto getLayout() const { return mLayout; }
	private:
		VkDescriptorSetLayout mLayout{ VK_NULL_HANDLE };
		Device::Ptr mDevice{ nullptr };
		std::vector<UniformParamenter::Ptr> mParams{};
	};

}