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
		glfwInit();

		// �ر� Opengl API, ���ý�ֹ�ı䴰�ڴ�С..
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
			//�����������¼�..
			glfwPollEvents();
		}
	}

	//��Դ����..
	void Application::clearUp() {
		mInstance.reset(); // ���� Vulkan�Ķ����������..
		glfwDestroyWindow(mWindow);

		//�˳�..
		glfwTerminate();
	}
}