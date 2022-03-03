#pragma once

#include"base.h"

namespace FF {
	class Application {
	public:
		Application() = default;

		~Application() = default;

		void run();
	private:
		void initWindow();

		void initVulkan();

		void mainLooper();

		void clearUp();

	private:
		GLFWwindow* mWindow { NULL };
	};
}