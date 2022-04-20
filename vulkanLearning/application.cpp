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
		mPipline = Wrapper::Pipline::create(mDevice);
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

		mSwapChain.reset();
		mDevice.reset();
		mWindowSurface.reset();
		mInstance.reset();
		mWindow.reset();
	}


	void Application::createPipeline()
	{
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
	}
}