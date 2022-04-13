#include"swapChain.h"

namespace FF::Wrapper {
	swapChain::swapChain(const WindowSurface::Ptr& windowSurface, const Window::Ptr& window, const Device::Dev_Ptr& device):
		mWindowSurface(windowSurface), mWindow(window), mDevice(device)
	{
		
	
	}

	swapChain::~swapChain() {
		if (mSwapChain != VK_NULL_HANDLE) {
			vkDestroySwapchainKHR(mDevice->getDevice(), mSwapChain, nullptr);
		}

		mWindow.reset();
		mWindowSurface.reset();
		mDevice.reset();
	}
}