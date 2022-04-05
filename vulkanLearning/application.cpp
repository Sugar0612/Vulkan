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
		mInstance = Wrapper::Instance::create(true);
	}

	void Application::mainLooper() {
		while (!glfwWindowShouldClose(mWindow)) {
			//解析鼠标键盘事件..
			glfwPollEvents();
		}
	}

	//资源回收..
	void Application::clearUp() {
		mInstance.reset(); // 关于 Vulkan的东西析构完毕..
		glfwDestroyWindow(mWindow);

		//退出..
		glfwTerminate();
	}
}