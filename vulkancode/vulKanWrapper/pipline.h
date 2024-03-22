#pragma once
#include "shader.h"
#include "device.h"
#include "../base.h"

namespace FF::Wrapper {
	
	class Pipeline {
	public:
		using Ptr = std::shared_ptr<Pipeline>;
		static Ptr create(const Device::Ptr& device) { return std::make_shared<Pipeline>(device); }

		Pipeline(const Device::Ptr &device);
		~Pipeline();

		void setShaderGroup(const std::vector<Shader::Ptr>& shaderGroup);

		void setViewport(const std::vector<VkViewport>& viewports) { mViewports = viewports; }

		void setScissors(const std::vector<VkRect2D>& scissors) { mScissors = scissors; }

		void pushBlendAttachment(const VkPipelineColorBlendAttachmentState& blendattachment) { 
			mBlendAttachmentStates.push_back(blendattachment); 
		}

		void bulid();
	public:
		VkPipelineVertexInputStateCreateInfo mVertexInputsState{};
		VkPipelineInputAssemblyStateCreateInfo mAssemblyState{};
		VkPipelineViewportStateCreateInfo mViewportState{};
		VkPipelineRasterizationStateCreateInfo mRasterState{};
		VkPipelineMultisampleStateCreateInfo mSampleState{};
		std::vector<VkPipelineColorBlendAttachmentState> mBlendAttachmentStates{};
		VkPipelineColorBlendStateCreateInfo mBlendState{};
		VkPipelineDepthStencilStateCreateInfo mDepthStencilState{};
		VkPipelineLayoutCreateInfo mLayoutState{};
	private:
		Device::Ptr mDevice{ nullptr };

		VkPipeline mPipline{ VK_NULL_HANDLE };

		VkPipelineLayout mLayout{ VK_NULL_HANDLE };
		std::vector<Shader::Ptr> mShaders{};
		std::vector<VkViewport> mViewports{};
		std::vector<VkRect2D> mScissors {};
	};
}