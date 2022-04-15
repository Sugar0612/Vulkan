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

		//选择合适的 SurfaceFormat...
		VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

		//选择一个合适的模式..
		VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

		//合适的窗口大小..
		VkExtent2D chooseExent(const VkSurfaceCapabilitiesKHR& availableCapabilities);

	private:
		VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevelsCount = 1);

	private:

		uint32_t mImageCount{ 0 };

		VkSwapchainKHR mSwapChain{ VK_NULL_HANDLE };

		VkFormat mSwapChainFormat;
		VkExtent2D mSwapChainExtent2D;

		std::vector<VkImage> mSwapChainImages{}; // image的创建与销毁，都要交给 swapChain..
		std::vector<VkImageView> mSwapChainImageViews{}; // Image管理器..

		WindowSurface::Ptr mWindowSurface{ nullptr };
		Window::Ptr mWindow{ nullptr };
		Device::Dev_Ptr mDevice{ nullptr };
	};
}