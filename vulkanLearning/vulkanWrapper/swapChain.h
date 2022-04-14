#pragma once
#include"../base.h"
#include"WindowSurface.h"
#include"window.h"
#include"device.h"

namespace FF::Wrapper {

	struct SwapChainSupportInfo {
		VkSurfaceCapabilitiesKHR mCapabilities;
		std::vector<VkSurfaceFormatKHR> mFormats;
		std::vector<VkPresentModeKHR> mPresentMode;
	};

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

		SwapChainSupportInfo querySwapChainSupportInfo();

		//ѡ����ʵ� SurfaceFormat...
		VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

		//ѡ��һ�����ʵ�ģʽ..
		VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

		//���ʵĴ��ڴ�С..
		VkExtent2D chooseExent(const VkSurfaceCapabilitiesKHR& availableCapabilities);

	private:

		VkSwapchainKHR mSwapChain{ VK_NULL_HANDLE };
		WindowSurface::Ptr mWindowSurface{ nullptr };
		Window::Ptr mWindow{ nullptr };
		Device::Dev_Ptr mDevice{ nullptr };
	};
}