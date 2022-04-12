#pragma once

#include"base.h"
#include"vulkanWrapper/instance.h"
#include "vulkanWrapper/device.h"
#include "vulkanWrapper/window.h"
#include "vulkanWrapper/WindowSurface.h"

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

		Wrapper::Window::Ptr mWindow{ nullptr };
		Wrapper::Instance::Inst_Ptr mInstance{ nullptr };
		Wrapper::Device::Dev_Ptr mDevice{ nullptr };
		Wrapper::WindowSurface::Ptr mWindowSurface{ nullptr };
	};
};