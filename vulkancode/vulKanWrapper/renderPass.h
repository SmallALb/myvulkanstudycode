#pragma once

#include "device.h"
#include "../base.h"




namespace FF::Wrapper {
	class Subpass {
	public:
		Subpass();

		~Subpass();

		void addColorAttachmentReference(const VkAttachmentReference& ref);

		void addInputAttachmentReference(const VkAttachmentReference& ref);

		void setDepthStencAttachmentReference(const VkAttachmentReference& ref);

		void bulidSubPassDescription();

		[[nodiscard]] auto getSubPassDescription() const { return mSubPassDescription; }

	private:
		VkSubpassDescription mSubPassDescription{};
		std::vector<VkAttachmentReference> mColorAttachmentReferences{};
		std::vector<VkAttachmentReference> mInputAttachmentReferences{};
		VkAttachmentReference mDpethStencilAttachmentReference{};
	};


	class RenderPass {
	public:
		using Ptr = std::shared_ptr<RenderPass>;

		static Ptr create(const Device::Ptr& device) {
			return std::make_shared<RenderPass>(device);
		}

		RenderPass(const Device::Ptr& device);

		~RenderPass();

		void addSubPass(const Subpass& subpass);

		void addDependency(const VkSubpassDependency& dependency);

		void addAttachment(const VkAttachmentDescription& attachmentDes);

		[[nodiscard]] auto getRenderPass() const { return mRenderPass; }

		void bulidRenderedPass();
	private:
		VkRenderPass mRenderPass{VK_NULL_HANDLE};
		std::vector<Subpass> mSubPasses{};
		std::vector<VkSubpassDependency> mDependencies{};
		std::vector<VkAttachmentDescription> mAttachmentDescriptions{};
		Device::Ptr mDevice{ nullptr };
	};

} 