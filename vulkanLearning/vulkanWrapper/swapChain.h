#pragma once
#include"../base.h"
#include"WindowSurface.h"
#include"window.h"
#include"device.h"

namespace FF::Wrapper {

	class swapChain {
		
	public:

		using Ptr = std::shared_ptr<swapChain>;

		static Ptr create(const WindowSurface::Ptr& windowSurface, 
			const Window::Ptr& window, 
			const Device::Dev_Ptr& device) 
		{ 
			return std::make_shared<swapChain>(windowSurface, window, device); 
		}

		swapChain(const WindowSurface::Ptr& windowSurface, const Window::Ptr& window, const Device::Dev_Ptr& device);

		~swapChain();

	private:

		VkSwapchainKHR mSwapChain{ VK_NULL_HANDLE };
		WindowSurface::Ptr mWindowSurface{ nullptr };
		Window::Ptr mWindow{ nullptr };
		Device::Dev_Ptr mDevice{ nullptr };

	};
}