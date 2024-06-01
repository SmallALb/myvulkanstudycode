#pragma once

#include "device.h"
#include "../base.h"
#include "description.h"
#include "descriptorLayout.h"
#include "descriptorPool.h"
namespace FF::Wrapper {
	class DescriptorSet {
	public:
		using Ptr = std::shared_ptr<DescriptorSet>;
		static Ptr create(
			const Device::Ptr& device,
			const std::vector<UniformParamenter::Ptr>& params,
			const DescriptorSetLayout::Ptr& layout,
			const DescriptorPool::Ptr & pool,
			int frameCount) { 
			return std::make_shared<DescriptorSet>(device, params, layout, pool, frameCount);
		}

		DescriptorSet(
			const Device::Ptr &device,
			const std::vector<UniformParamenter::Ptr>& params,
			const DescriptorSetLayout::Ptr& layout,
			const DescriptorPool::Ptr & pool,
			int frameCount
			);

		~DescriptorSet();

		[[nodiscard]] auto getDescriptorSet(int frameCount) const { return mDescriptorSets[frameCount]; }
	
	private:
		std::vector<VkDescriptorSet> mDescriptorSets{};
		Device::Ptr mDevice{nullptr};
	};
}