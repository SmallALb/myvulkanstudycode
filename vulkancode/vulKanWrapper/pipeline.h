#pragma once
#include "shader.h"
#include "device.h"
#include "../base.h"
#include "renderPass.h"

namespace FF::Wrapper {
	
	class Pipeline {
	public:
		using Ptr = std::shared_ptr<Pipeline>;
		static Ptr create(const Device::Ptr& device, const RenderPass::Ptr& renderpass) { return std::make_shared<Pipeline>(device, renderpass); }

		Pipeline(const Device::Ptr &device, const RenderPass::Ptr &renderpass);
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

	public:
		[[nodiscard]] auto getPipeline() const { return mPipline; }

	private:
		Device::Ptr mDevice{ nullptr };

		VkPipeline mPipline{ VK_NULL_HANDLE };

		RenderPass::Ptr mRenderPass{ nullptr };

		VkPipelineLayout mLayout{ VK_NULL_HANDLE };
		std::vector<Shader::Ptr> mShaders{};
		std::vector<VkViewport> mViewports{};
		std::vector<VkRect2D> mScissors {};
	};
}