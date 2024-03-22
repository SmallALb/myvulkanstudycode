#pragma once
#include "device.h"
#include "../base.h"

namespace FF::Wrapper {

	class Shader {
	public:
		using Ptr = std::shared_ptr<Shader>;
		static Ptr create(const Device::Ptr& device, const std::string& fileName, VkShaderStageFlagBits shaderStages, const std::string& entryPoint) {
			return std::make_shared<Shader>(device, fileName, shaderStages,entryPoint);
		}

		Shader(const Device::Ptr& device, const std::string &fileName, VkShaderStageFlagBits shaderStages, const std::string &entryPoint);
		~Shader();

		[[nodiscard]] auto getShaderStage() const { return mShaderStage; }
		[[nodiscard]] auto getShaderEntryPoint() const { return mEntryPoint.c_str(); }
		[[nodiscard]] auto getShaderModule() const { return mShaderModule; }

	private:
		VkShaderModule mShaderModule{VK_NULL_HANDLE};
		
		Device::Ptr mDevice{ nullptr };

		std::string mEntryPoint;

		VkShaderStageFlagBits mShaderStage;
	};

}