#include"application.h"

namespace FF {

	void Application::run() {
		initWindow();
		initVulkan();
		mainLooper();
		clearUp();
	}

	void Application::initWindow() {
		glfwInit();

		// 关闭 Opengl API, 设置禁止改变窗口大小..
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		mWindow = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan window", nullptr, nullptr);

		if (!mWindow) {
			std::cout << "Error: Failed Create Window!" << std::endl;
		}
	}

	void Application::initVulkan() {
		mInstance = Wrapper::Instance::create();
	}

	void Application::mainLooper() {
		while (!glfwWindowShouldClose(mWindow)) {
			glfwPollEvents();
		}
	}

	void Application::clearUp() {
		glfwDestroyWindow(mWindow);
		glfwTerminate();
	}
}