#pragma once

#include"base.h"
#include"vulkanWrapper/instance.h"
#include "vulkanWrapper/device.h"
#include "vulkanWrapper/window.h"
#include "vulkanWrapper/WindowSurface.h"
#include "vulkanWrapper/swapChain.h"
#include "vulkanWrapper/Shader.h"
#include "vulkanWrapper/pipline.h"

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
		void createPipeline();

	private:

		Wrapper::Window::Ptr mWindow{ nullptr };
		Wrapper::Instance::Inst_Ptr mInstance{ nullptr };
		Wrapper::Device::Dev_Ptr mDevice{ nullptr };
		Wrapper::WindowSurface::Ptr mWindowSurface{ nullptr };
		Wrapper::swapChain::Ptr mSwapChain{ nullptr };
		Wrapper::Pipline::Ptr mPipline{ nullptr };

	};
};