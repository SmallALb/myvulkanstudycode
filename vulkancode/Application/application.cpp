#include "application.h"

void FF::Application::run() {
	initWindow();
	initVulKan();
	mainLoop();
	cleanUp();
}

void FF::Application::initWindow() {
	mWindow = Wrapper::Window::create(WIDTH, HEIGHT);
}

void FF::Application::initVulKan(){
	mInstance = Wrapper::Instance::create(true);
	mWindowSurface = Wrapper::WindowSurface::create(mInstance, mWindow);
	mDevice = Wrapper::Device::create(mInstance, mWindowSurface);
	mSwapChain = Wrapper::SwapChain::create(mDevice,mWindow, mWindowSurface);
	mPipeline = Wrapper::Pipeline::create(mDevice);
	createPipeline();
}

void FF::Application::mainLoop() {
	while (mWindow->shouldClose()) {
		mWindow->pollEvents();
	}

}

void FF::Application::cleanUp() {
	mPipeline.reset();
	mSwapChain.reset();
	mDevice.reset();
	mWindowSurface.reset();
	mInstance.reset();
	mWindow.reset();
}

void FF::Application::createPipeline() {
	VkViewport viewport = {};
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = (float)WIDTH;
	viewport.height = (float)HEIGHT;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0,0 };
	scissor.extent = { WIDTH, HEIGHT };

	mPipeline->setViewport({ viewport });
	mPipeline->setScissors({ scissor });
	
	auto sharderVertex = Wrapper::Shader::create(mDevice, "shaders/vs.spv", VK_SHADER_STAGE_VERTEX_BIT, "main");
	auto sharderfragmnt = Wrapper::Shader::create(mDevice, "shaders/fs.spv", VK_SHADER_STAGE_FRAGMENT_BIT, "main");
	std::vector<Wrapper::Shader::Ptr> shaderGroup = {sharderVertex, sharderfragmnt};

	std::vector<VkPipelineShaderStageCreateInfo> shaderCreateInfos{};

	for (const auto& shader : shaderGroup) {
		VkPipelineShaderStageCreateInfo shaderCreateInfo{};
		shaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderCreateInfo.stage = shader->getShaderStage();
		shaderCreateInfo.pName = shader->getShaderEntryPoint();
		shaderCreateInfo.module = shader->getShaderModule();

		shaderCreateInfos.push_back(shaderCreateInfo);
	}

	mPipeline->setShaderGroup(shaderGroup);

	mPipeline->mVertexInputsState.vertexBindingDescriptionCount = 0;
	mPipeline->mVertexInputsState.pVertexBindingDescriptions = nullptr;
	mPipeline->mVertexInputsState.vertexAttributeDescriptionCount = 0;
	mPipeline->mVertexInputsState.pVertexAttributeDescriptions = nullptr;

	mPipeline->mAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	mPipeline->mAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	mPipeline->mAssemblyState.primitiveRestartEnable = VK_FALSE;

	mPipeline->mRasterState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	mPipeline->mRasterState.polygonMode = VK_POLYGON_MODE_FILL;
	mPipeline->mRasterState.lineWidth = 1.0f;
	mPipeline->mRasterState.cullMode = VK_CULL_MODE_BACK_BIT;
	mPipeline->mRasterState.frontFace = VK_FRONT_FACE_CLOCKWISE;

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

}

