#include "renderPass.h"

namespace FF::Wrapper {
	Subpass::Subpass() {
	
	}

	Subpass::~Subpass() {
	
	}

	void Subpass::addColorAttachmentReference(const VkAttachmentReference& ref) {
		mColorAttachmentReferences.push_back(ref);
	}

	void Subpass::addInputAttachmentReference(const VkAttachmentReference& ref){
		mInputAttachmentReferences.push_back(ref);
	}

	void Subpass::setDepthStencAttachmentReference(const VkAttachmentReference& ref) {
		mDpethStencilAttachmentReference = ref;
	}

	void Subpass::bulidSubPassDescription() {
		if (mColorAttachmentReferences.empty()) {
			throw std::runtime_error("Error: color attachment grop is empty!");
		}
		mSubPassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		mSubPassDescription.colorAttachmentCount = static_cast<uint32_t>(mColorAttachmentReferences.size());
		mSubPassDescription.pColorAttachments = mColorAttachmentReferences.data();

		mSubPassDescription.inputAttachmentCount = static_cast<uint32_t>(mInputAttachmentReferences.size());
		mSubPassDescription.pInputAttachments = mInputAttachmentReferences.data();

		mSubPassDescription.pDepthStencilAttachment = mDpethStencilAttachmentReference.layout == VK_IMAGE_LAYOUT_UNDEFINED ? nullptr : &mDpethStencilAttachmentReference;
	}

	RenderPass::RenderPass(const Device::Ptr & decvice) {
		mDevice = decvice;
	}

	RenderPass::~RenderPass() {
		if (mRenderPass != VK_NULL_HANDLE) vkDestroyRenderPass(mDevice->getDevice(), mRenderPass, nullptr);

	}
	void RenderPass::addSubPass(const Subpass& subpass) {
		mSubPasses.push_back(subpass);
	}
	void RenderPass::addDependency(const VkSubpassDependency& dependency) {
		mDependencies.push_back(dependency);
	}
	void RenderPass::addAttachment(const VkAttachmentDescription& attachmentDes) {
		mAttachmentDescriptions.push_back(attachmentDes);
	}
	void RenderPass::bulidRenderedPass() {
		if (mSubPasses.empty() || mAttachmentDescriptions.empty() || mDependencies.empty()) {
			throw std::runtime_error("Error : not enought elements to bulid renderPass");
		}

		std::vector<VkSubpassDescription> subPasses{};
		for (int i = 0; i < mSubPasses.size(); i++) {
			subPasses.push_back(mSubPasses[i].getSubPassDescription());
		}

		VkRenderPassCreateInfo createInfo{};

		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.attachmentCount = static_cast<uint32_t>(mAttachmentDescriptions.size());
		createInfo.pAttachments = mAttachmentDescriptions.data();
		createInfo.dependencyCount = static_cast<uint32_t>(mDependencies.size());
		createInfo.pDependencies = mDependencies.data();
		createInfo.subpassCount = static_cast<uint32_t>(subPasses.size());
		createInfo.pSubpasses = subPasses.data();

		if (vkCreateRenderPass(mDevice->getDevice(), &createInfo, nullptr, &mRenderPass) != VK_SUCCESS) {
			throw std::runtime_error("Error: failed to create renderPass");
		}
	}
}