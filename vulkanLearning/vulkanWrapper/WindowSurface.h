#pragma once

#include "../base.h"
#include "instance.h"
#include "window.h"

namespace FF::Wrapper {
	class WindowSurface {

	public:

		using Ptr = std::shared_ptr<WindowSurface>;
		static Ptr create(Instance::Inst_Ptr instance, Window::Ptr window) { return std::make_shared<WindowSurface>(instance, window); }

		WindowSurface(Instance::Inst_Ptr instance, Window::Ptr window);

		[[nodiscard]] auto getWindowSurface() const { return mSurface; }

		~WindowSurface();

	private:
		VkSurfaceKHR mSurface{ VK_NULL_HANDLE };

		Instance::Inst_Ptr mInstance{ nullptr };
		Window::Ptr mWindow{ nullptr };

	};
}