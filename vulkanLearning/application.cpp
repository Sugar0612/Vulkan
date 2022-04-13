#include"application.h"

namespace FF {

	// run��һ�ε�����Щ����..
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
	}

	void Application::mainLooper() {
		while (!mWindow->WindowShouldClose()) {
			//�����������¼�..
			//glfwPollEvents();
			mWindow->PollEvents();
		}
	}

	//��Դ����..
	void Application::clearUp() {
		// ���� Vulkan�Ķ����������..

		mSwapChain.reset();
		mDevice.reset();
		mWindowSurface.reset();
		mInstance.reset();
		mWindow.reset();
	}
}