#include"window.h"

namespace FF::Wrapper {
	Window::Window(const int& Width, const int& height):
		mWidth(Width), mHeight(height)
	{
		glfwInit();

		// �ر� Opengl API, ���ý�ֹ�ı䴰�ڴ�С..
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		mWindow = glfwCreateWindow(mWidth, mHeight, "Vulkan window", nullptr, nullptr);

		if (!mWindow) {
			std::cout << "Error: Failed Create Window!" << std::endl;
		}
	}

	bool Window::WindowShouldClose()
	{
		return glfwWindowShouldClose(mWindow);
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	Window::~Window() {

		glfwDestroyWindow(mWindow);

		//�˳�..
		glfwTerminate();
	}
};