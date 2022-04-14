#include"swapChain.h"

namespace FF::Wrapper {
	swapChain::swapChain(const WindowSurface::Ptr& windowSurface, const Window::Ptr& window, const Device::Dev_Ptr& device):
		mWindowSurface(windowSurface), mWindow(window), mDevice(device)
	{
		
		auto swapChainSupportInfo = querySwapChainSupportInfo();
	}

	swapChain::~swapChain() {
		if (mSwapChain != VK_NULL_HANDLE) {
			vkDestroySwapchainKHR(mDevice->getDevice(), mSwapChain, nullptr);
		}

		mWindow.reset();
		mWindowSurface.reset();
		mDevice.reset();
	}


	SwapChainSupportInfo swapChain::querySwapChainSupportInfo()
	{

		//获得基础特性
		SwapChainSupportInfo info;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mDevice->getPhysicalDevice(), mWindowSurface->getWindowSurface(), &info.mCapabilities);


		//获得表面格式..
		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(mDevice->getPhysicalDevice(), mWindowSurface->getWindowSurface(), &formatCount, nullptr);

		if (formatCount != 0) {
			info.mFormats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(mDevice->getPhysicalDevice(), mWindowSurface->getWindowSurface(), &formatCount, info.mFormats.data());
		}

		//获取可用呈现模式..

		uint32_t presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(mDevice->getPhysicalDevice(), mWindowSurface->getWindowSurface(), &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			info.mPresentMode.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(mDevice->getPhysicalDevice(), mWindowSurface->getWindowSurface(), &presentModeCount, info.mPresentMode.data());
		}

		return info;
	}


	VkSurfaceFormatKHR swapChain::chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		//如果返回的是一个没有的格式，那么我们需要自己做一个..
		if (1 == availableFormats.size() && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
			return { VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
		}

		for (const auto& availableFormat: availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
				availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}

		}

		return availableFormats[0];
	}

	VkPresentModeKHR swapChain::choosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		// FIFO在设备上被绝对支持，如果在移动设备上，考虑节省电源消耗，优先选择FIFO..
		VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

		for (const auto& availablePresentMode : availablePresentModes) {

			//如果是高性能模式..
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}

		return bestMode;
	}


	VkExtent2D swapChain::chooseExent(const VkSurfaceCapabilitiesKHR& availableCapabilities)
	{
		//一下情况不可以自己设定Exent..
		if (availableCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return availableCapabilities.currentExtent;
		}

		//对于高清屏幕，窗体大小不等于像素长宽..
		int width = 0, height = 0;
		glfwGetFramebufferSize(mWindow->getGlfwWindow(), &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		//长宽在 max 与 min 之间..
	
		actualExtent.width = std::max(availableCapabilities.minImageExtent.width, 
			std::min(availableCapabilities.maxImageExtent.width, actualExtent.width));

		actualExtent.height = std::max(availableCapabilities.minImageExtent.height,
			std::min(availableCapabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}