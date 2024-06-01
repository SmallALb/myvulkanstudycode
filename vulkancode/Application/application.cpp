#include "application.h"

void FF::Application::run() {
	initWindow();
	initVulKan();
	mainLoop();
	cleanUp();
}

void FF::Application::initWindow() {
	mWindow = Wrapper::Window::create(mWidth, mHeight);
}

void FF::Application::initVulKan(){
	mInstance = Wrapper::Instance::create(true);
	mWindowSurface = Wrapper::WindowSurface::create(mInstance, mWindow);
	mDevice = Wrapper::Device::create(mInstance, mWindowSurface);
	mSwapChain = Wrapper::SwapChain::create(mDevice,mWindow, mWindowSurface);

	mWidth = mSwapChain->getExtent().width;
	mHeight = mSwapChain->getExtent().height;

	mModel = Model::create(mDevice);

	
	mRenderPass = Wrapper::RenderPass::create(mDevice);

	createRenderPass();

	mSwapChain->createFrameBuffers(mRenderPass);


	mUniformManager = UniformManager::create();
	mUniformManager->init(mDevice, mSwapChain->getImageCount());

	mPipeline = Wrapper::Pipeline::create(mDevice, mRenderPass);
	createPipeline();

	mCommandPool = Wrapper::CommandPool::create(mDevice);

	mCommandBuffers.resize(mSwapChain->getImageCount());

	createCommandBuffers();

	createSyncObjects();
	
	
}

void FF::Application::mainLoop() {
	while (mWindow->shouldClose()) {
		mWindow->pollEvents();
		mModel->update();
		mUniformManager->update(mVPMatrices, mModel->getUniform(), mCurrentFrame);

		render();
	}

	vkDeviceWaitIdle(mDevice->getDevice());
}

void FF::Application::cleanUp() {
	mPipeline.reset();

	mRenderPass.reset();

	mSwapChain.reset();
	mDevice.reset(); 
	mWindowSurface.reset();
	mInstance.reset();
	mWindow.reset();
}

void FF::Application::createPipeline() {
	VkViewport viewport = {};
	viewport.x = 0;
	viewport.y =  0;
	viewport.width = (float)mWidth;
	viewport.height = (float)mHeight;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0,0 };
	scissor.extent = { mWidth, mHeight };

	mPipeline->setViewport({ viewport });
	mPipeline->setScissors({ scissor });
	
	auto sharderVertex = Wrapper::Shader::create(mDevice, "E:/code/mVulkanCode/vulkancode/shaders/vs.spv", VK_SHADER_STAGE_VERTEX_BIT, "main");
	auto sharderfragmnt = Wrapper::Shader::create(mDevice, "E:/code/mVulkanCode/vulkancode/shaders/fs.spv", VK_SHADER_STAGE_FRAGMENT_BIT, "main");
	std::vector<Wrapper::Shader::Ptr> shaderGroup = {sharderVertex, sharderfragmnt};

	std::vector<VkPipelineShaderStageCreateInfo> shaderCreateInfos{};

	for (const auto& shader : shaderGroup) {
		VkPipelineShaderStageCreateInfo shaderCreateInfo{};
		shaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderCreateInfo.stage = shader->getShaderStage();
		shaderCreateInfo.pName = shader->getShaderEntryPoint().c_str();
		shaderCreateInfo.module = shader->getShaderModule();

		shaderCreateInfos.push_back(shaderCreateInfo);
	}
	
	mPipeline->setShaderGroup(shaderGroup);

	auto vertexBindingDes = mModel->getVertexInputBindingDescription();
	auto attributions = mModel->getAttributeDescriptions();
	mPipeline->mVertexInputsState.vertexBindingDescriptionCount = vertexBindingDes.size();
	mPipeline->mVertexInputsState.pVertexBindingDescriptions = vertexBindingDes.data();
	mPipeline->mVertexInputsState.vertexAttributeDescriptionCount = attributions.size();
	mPipeline->mVertexInputsState.pVertexAttributeDescriptions = attributions.data();

	mPipeline->mAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	mPipeline->mAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	mPipeline->mAssemblyState.primitiveRestartEnable = VK_FALSE;

	mPipeline->mRasterState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	mPipeline->mRasterState.polygonMode = VK_POLYGON_MODE_FILL;
	mPipeline->mRasterState.lineWidth = 1.0f;
	//mPipeline->mRasterState.cullMode = VK_CULL_MODE_BACK_BIT;
	//mPipeline->mRasterState.frontFace = VK_FRONT_FACE_CLOCKWISE;

	mPipeline->mRasterState.depthBiasEnable = VK_FALSE;
	mPipeline->mRasterState.depthBiasConstantFactor = 0.0f;
	mPipeline->mRasterState.depthBiasClamp = 0.0f;
	mPipeline->mRasterState.depthBiasSlopeFactor = 0.0f;

	mPipeline->mSampleState.sampleShadingEnable = VK_FALSE;
	mPipeline->mSampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	mPipeline->mSampleState.minSampleShading = 1.0f;
	mPipeline->mSampleState.pSampleMask = nullptr;
	mPipeline->mSampleState.alphaToCoverageEnable = VK_FALSE;
	mPipeline->mSampleState.alphaToOneEnable = VK_FALSE;


	VkPipelineColorBlendAttachmentState blendAttachment{};
	blendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	blendAttachment.blendEnable = VK_FALSE;
	blendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	blendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	blendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	
	blendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	blendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	blendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	mPipeline->pushBlendAttachment(blendAttachment);

	mPipeline->mBlendState.logicOpEnable = VK_FALSE;
	mPipeline->mBlendState.logicOp = VK_LOGIC_OP_COPY;

	mPipeline->mBlendState.blendConstants[0] = 0.0f;
	mPipeline->mBlendState.blendConstants[1] = 0.0f;
	mPipeline->mBlendState.blendConstants[2] = 0.0f;
	mPipeline->mBlendState.blendConstants[3] = 0.0f;

	mPipeline->mLayoutState.setLayoutCount = 1;

	auto layout = mUniformManager->getDescriptorLayout()->getLayout();

	mPipeline->mLayoutState.pSetLayouts = &layout;
	mPipeline->mLayoutState.pushConstantRangeCount = 0;
	mPipeline->mLayoutState.pPushConstantRanges = nullptr;

	mPipeline->bulid();
}

void FF::Application::createRenderPass() {
	VkAttachmentDescription attachmentDes{};
	attachmentDes.format = mSwapChain->getFormat();
	attachmentDes.samples = VK_SAMPLE_COUNT_1_BIT;
	attachmentDes.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDes.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachmentDes.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDes.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDes.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachmentDes.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	mRenderPass->addAttachment(attachmentDes);

	VkAttachmentReference attachmentRef{};
	attachmentRef.attachment = 0;
	attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	Wrapper::Subpass subpass{};
	subpass.addColorAttachmentReference(attachmentRef);
	subpass.bulidSubPassDescription();

	mRenderPass->addSubPass(subpass);

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	mRenderPass->addDependency(dependency);

	mRenderPass->bulidRenderedPass() ;
}

void FF::Application::createCommandBuffers() {
	for (int i = 0; i < mSwapChain->getImageCount(); i++) {
		mCommandBuffers[i] = Wrapper::CommandBuffer::create(mDevice, mCommandPool);

		mCommandBuffers[i]->begin();

		VkRenderPassBeginInfo renderBeginInfo{};
		renderBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderBeginInfo.renderPass = mRenderPass->getRenderPass();
		renderBeginInfo.framebuffer = mSwapChain->getFrameBuffer(i);
		renderBeginInfo.renderArea.offset = { 0, 0 };
		renderBeginInfo.renderArea.extent = mSwapChain->getExtent();

		VkClearValue clearColor{ 0.0, 0.0, 0.0, 1.0 };
		renderBeginInfo.clearValueCount = 1;
		renderBeginInfo.pClearValues = &clearColor;


		mCommandBuffers[i]->beginRenderPass(renderBeginInfo);

		mCommandBuffers[i]->bindGraphicPipeline(mPipeline->getPipeline());

		mCommandBuffers[i]->bindDescriptorSet(mPipeline->getLayout(), mUniformManager->getDescriptorset(mCurrentFrame));

		mCommandBuffers[i]->bindVertexBuffer(mModel->getVertexBuffers());

		mCommandBuffers[i]->bindIndexBuffer(mModel->getIndexBuffer()->getBuffer());

		mCommandBuffers[i]->drawIndex(mModel->getIndexCount());

		mCommandBuffers[i]->endRenderPass();

		mCommandBuffers[i]->end();
	}

}

void FF::Application::createSyncObjects() {
	for (int i = 0; i < mSwapChain->getImageCount(); i++) {
		auto imageSemaphore = Wrapper::Semaphore::create(mDevice);
		mImageAvailableSemaphores.push_back(imageSemaphore);

		auto renderSemaphore = Wrapper::Semaphore::create(mDevice);
		mRenderFinishedSemaphores.push_back(renderSemaphore);

		auto fence = Wrapper::Fence::create(mDevice);
		mFences.push_back(fence);
	}
}

void FF::Application::cleanupSwapChain() {
	mSwapChain.reset();
	mCommandBuffers.clear();
	mPipeline.reset();
	mRenderPass.reset();
	mImageAvailableSemaphores.clear();
	mRenderFinishedSemaphores.clear();
	mFences.clear();
}




void FF::Application::recreateSwapChain() {
	int width = 0, height = 0;
	glfwGetFramebufferSize(mWindow->getWindow(), &width, &height);

	while (width == 0 || height == 0) {
		glfwWaitEvents();
		glfwGetFramebufferSize(mWindow->getWindow(), &width, &height);
	}

	vkDeviceWaitIdle(mDevice->getDevice());
	cleanupSwapChain();

	mSwapChain = Wrapper::SwapChain::create(mDevice, mWindow, mWindowSurface);

	mRenderPass = Wrapper::RenderPass::create(mDevice);

	createRenderPass();

	mSwapChain->createFrameBuffers(mRenderPass);
	mWidth = mSwapChain->getExtent().width;
	mHeight = mSwapChain->getExtent().height;

	mPipeline = Wrapper::Pipeline::create(mDevice, mRenderPass);

	createPipeline();

	mCommandPool = Wrapper::CommandPool::create(mDevice);

	mCommandBuffers.resize(mSwapChain->getImageCount());

	createCommandBuffers();

	createSyncObjects();

}

void FF::Application::render() {
	mFences[mCurrentFrame]->block();


	uint32_t imageIndex = 0;
	VkResult result = vkAcquireNextImageKHR(mDevice->getDevice(),
		mSwapChain->getSwapChain(), 
		UINT64_MAX, 
		mImageAvailableSemaphores[mCurrentFrame]->getSemaphore(),
		VK_NULL_HANDLE,
		&imageIndex
		);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
		mWindow->windowresize = false;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("Error: failed to acquire next image");
	}

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {mImageAvailableSemaphores[mCurrentFrame]->getSemaphore()};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	auto commandBuffer = mCommandBuffers[imageIndex]->getCommandBuffer();
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	VkSemaphore signalSemaphores[] = { mRenderFinishedSemaphores[mCurrentFrame]->getSemaphore() };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	mFences[mCurrentFrame]->resetFence();
	if (vkQueueSubmit(mDevice->getGraphicQueue(), 1, &submitInfo, mFences[mCurrentFrame]->getFence()) != VK_SUCCESS) {
		throw std::runtime_error("Error: failed to subit renderCommand");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { mSwapChain->getSwapChain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(mDevice->getPresentQueue(), &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || mWindow->windowresize) {
		recreateSwapChain();
		mWindow->windowresize = 0;
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("Error: failed to present");
	}

	mCurrentFrame = (mCurrentFrame + 1) % mSwapChain->getImageCount();
}

