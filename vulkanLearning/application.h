#pragma once

#include"base.h"
#include"vulkanWrapper/instance.h"
#include "vulkanWrapper/device.h"

namespace FF {

	const int WIDTH = 800;
	const int HEIGHT = 600;

	class Application {

	public:
		Application() = default;

		~Application() = default;

		void run();

		void initWindow();

		void initVulkan();

		void mainLooper();

		void clearUp();

	private:
		GLFWwindow* mWindow = nullptr;
		Wrapper::Instance::Inst_Ptr mInstance;
		Wrapper::Device::Dev_Ptr mDevice;
	};
};