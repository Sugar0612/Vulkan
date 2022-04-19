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
		mShader = Wrapper::Shader::create(mDevice, "./Shader/vert.spv", VK_SHADER_STAGE_VERTEX_BIT, "main");
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

		mShader.reset();
		mSwapChain.reset();
		mDevice.reset();
		mWindowSurface.reset();
		mInstance.reset();
		mWindow.reset();
	}
}