#pragma once

#include "buffer.h"

namespace FF::Wrapper {
	struct UniformParamenter {
		using Ptr = std::shared_ptr<UniformParamenter>;
		static Ptr create() { return std::make_shared<UniformParamenter>(); }

		size_t mSize{ 0 };
		uint32_t mBinding{ 0 };
		uint32_t mCount{ 0 };
		VkDescriptorType mDescriptorType;
		VkShaderStageFlagBits mStage;

		std::vector<Buffer::Ptr> mBuffers{};
	};

}