#pragma once


#include "../base.h"

namespace FF::Wrapper {

	class Window {

	public:

		using Ptr = std::shared_ptr<Window>;

		static Ptr create(const int& Width, const int& Height) { return std::make_shared<Window>(Width, Height); }

		Window(const int& Width, const int& height);

		~Window();

		bool WindowShouldClose();

		void PollEvents();

		[[nodiscard]] auto getGlfwWindow() const { return mWindow; }

	private:

		int mWidth{ 0 };
		int mHeight{ 0 };
		GLFWwindow* mWindow{ nullptr };
	};
};