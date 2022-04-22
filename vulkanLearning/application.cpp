#include"application.h"

namespace FF {

	// run会一次调用这些函数..
	void Application::run() {
		initWindow();
		initVulkan();
		mainLooper();
		clearUp();
	}

	void Application::initWindow() {
		mWindow = Wrapper::Window::create(WIDTH, HEIGHT);
	}

	void Application::initVulkan() {
		mInstance = Wrapper::Instance::create(true);
		mWindowSurface = Wrapper::WindowSurface::create(mInstance, mWindow);
		mDevice = Wrapper::Device::create(mInstance, mWindowSurface);
		mSwapChain = Wrapper::swapChain::create(mWindowSurface, mWindow, mDevice);

		mRenderPass = Wrapper::RenderPass::create(mDevice);
		createRenderPass();

		mSwapChain->createFrameBuffers(mRenderPass);

		mPipline = Wrapper::Pipline::create(mDevice, mRenderPass);
		createPipeline();
	}

	void Application::mainLooper() {
		while (!mWindow->WindowShouldClose()) {
			//解析鼠标键盘事件..
			//glfwPollEvents();
			mWindow->PollEvents();
		}
	}

	//资源回收..
	void Application::clearUp() {
		// 关于 Vulkan的东西析构完毕..

		mPipline.reset();
		mRenderPass.reset();
		mSwapChain.reset();
		mDevice.reset();
		mWindowSurface.reset();
		mInstance.reset();
		mWindow.reset();
	}


	void Application::createPipeline()
	{
		//设置视口..
		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)WIDTH;
		viewport.height = (float)HEIGHT;

		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor = {};
		scissor.offset = { 0, 0 };
		scissor.extent = { WIDTH, HEIGHT };
		
		mPipline->setViewports({ viewport });
		mPipline->setScissors({ scissor });

		//设置Shader..
		std::vector<Wrapper::Shader::Ptr> shaderGroup{};

		auto shaderVertex = Wrapper::Shader::create(mDevice, "Shader/vert.spv", VK_SHADER_STAGE_VERTEX_BIT, "main");
		shaderGroup.push_back(shaderVertex);

		auto shaderFragment = Wrapper::Shader::create(mDevice, "Shader/frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT, "main");
		shaderGroup.push_back(shaderFragment);

		mPipline->setShaderGroup(shaderGroup);

		//顶点排布模式..
		mPipline->mVertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		mPipline->mVertexInputState.vertexBindingDescriptionCount = 0;
		mPipline->mVertexInputState.pVertexBindingDescriptions = nullptr;
		mPipline->mVertexInputState.vertexAttributeDescriptionCount = 0;
		mPipline->mVertexInputState.pVertexAttributeDescriptions = nullptr;

		//图源装配..
		mPipline->mAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		mPipline->mAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; //拓扑..
		mPipline->mAssemblyState.primitiveRestartEnable = VK_FALSE;

		//光栅化..
		mPipline->mRasterState.polygonMode = VK_POLYGON_MODE_FILL; // 其他模式需要开启 gpu..
		mPipline->mRasterState.lineWidth = 1.0f; // 大于1 需要开启 gpu..
		mPipline->mRasterState.cullMode = VK_CULL_MODE_BACK_BIT;
		mPipline->mRasterState.frontFace = VK_FRONT_FACE_CLOCKWISE;
		mPipline->mRasterState.depthBiasEnable = VK_FALSE; // depthBiasClamp可以设置一些关于z轴遮挡的数值..目前是关闭的..
		mPipline->mRasterState.depthBiasConstantFactor = 0.0f;
		mPipline->mRasterState.depthBiasClamp = 0.0f;
		mPipline->mRasterState.depthBiasSlopeFactor = 0.0f;

		//多重采样..
		mPipline->mSampleState.sampleShadingEnable = VK_FALSE;
		mPipline->mSampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		mPipline->mSampleState.minSampleShading = 1.0f;
		mPipline->mSampleState.pSampleMask = nullptr;
		mPipline->mSampleState.alphaToCoverageEnable = VK_FALSE;
		mPipline->mSampleState.alphaToOneEnable = VK_FALSE;

		//颜色混合.. alpha基础运算..
		VkPipelineColorBlendAttachmentState blendAttachment;

		blendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
			VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT; //得到颜色混合结果，按照通道掩码 and 操作输出..

		blendAttachment.blendEnable = VK_FALSE;
		blendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		blendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		blendAttachment.colorBlendOp = VK_BLEND_OP_ADD;

		blendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		blendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		blendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		mPipline->mBlendAttachmentStates.push_back(blendAttachment);

		// 颜色混合位运算..

		mPipline->mBlendState.logicOpEnable = VK_FALSE; // 如果开启 logicOp 那么基于 alpha的基础运算失效..
		mPipline->mBlendState.logicOp = VK_LOGIC_OP_COPY;

		//配合 blendAttachment的 Factor 与 op.. RGBA..(与图片的 alpha混合有关..)
		mPipline->mBlendState.blendConstants[0] = 0.f;
		mPipline->mBlendState.blendConstants[1] = 0.f;
		mPipline->mBlendState.blendConstants[2] = 0.f;
		mPipline->mBlendState.blendConstants[3] = 0.f;

		// uniform传递..
		mPipline->mLayoutState.setLayoutCount = 0;
		mPipline->mLayoutState.pSetLayouts = nullptr;
		mPipline->mLayoutState.pushConstantRangeCount = 0;
		mPipline->mLayoutState.pPushConstantRanges = nullptr;

		//mPipline->build();
	}

	void Application::createRenderPass()
	{
		VkAttachmentDescription attachmentDes{};
		attachmentDes.format = mSwapChain->getFormat();
		attachmentDes.samples = VK_SAMPLE_COUNT_1_BIT;
		attachmentDes.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachmentDes.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // 输出..
		attachmentDes.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachmentDes.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachmentDes.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachmentDes.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		mRenderPass->addAttachmentDescription(attachmentDes);

		VkAttachmentReference attachmentRef{};
		attachmentRef.attachment = 0;
		attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		Wrapper::SubPass subPass{};
		subPass.addColorAttachmentReference(attachmentRef);
		subPass.buildSubPassDesription();

		mRenderPass->addSubPass(subPass);

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		mRenderPass->addSubPassDependency(dependency);
		mRenderPass->buildRenderPass();
	}
}