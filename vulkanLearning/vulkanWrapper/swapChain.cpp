#include"swapChain.h"

namespace FF::Wrapper {
	swapChain::swapChain(const WindowSurface::Ptr& windowSurface, const Window::Ptr& window, const Device::Dev_Ptr& device):
		mWindowSurface(windowSurface), mWindow(window), mDevice(device)
	{
		
		auto swapChainSupportInfo = querySwapChainSupportInfo();

		// 开始 info的筛选..
		VkSurfaceFormatKHR surfaceFormat = chooseSurfaceFormat(swapChainSupportInfo.mFormats);
		VkPresentModeKHR presentMode = choosePresentMode(swapChainSupportInfo.mPresentMode);
		VkExtent2D extent = chooseExent(swapChainSupportInfo.mCapabilities);

		//设置图像缓冲数量.. (如果 maxImageCount = 0, 说明内存不爆栈的情况下, imageCount可以随意大小..)
		uint32_t mImageCount = swapChainSupportInfo.mCapabilities.minImageCount + 1;
		if (swapChainSupportInfo.mCapabilities.maxImageCount > 0 && mImageCount > swapChainSupportInfo.mCapabilities.maxImageCount) {
			mImageCount = swapChainSupportInfo.mCapabilities.maxImageCount;
		}

		//填写创建信息..
		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = mWindowSurface->getWindowSurface();
		createInfo.minImageCount = mImageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageExtent = extent;

		// VR = 2..
		createInfo.imageArrayLayers = 1;

		//交换链生成的图片 用于 何处..
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		//交换链生成的图片可以用来 渲染 和 显示，而渲染 和 显示应用不同的队列...
		//...所以需要设置让交换链的图片被两个队列兼容..

		std::vector<uint32_t> queueFamilies = { mDevice->getGraphicQueueFamily().value(), mDevice->getPresentQueueFamily().value() };

		if (mDevice->getGraphicQueueFamily().value() == mDevice->getPresentQueueFamily().value()) {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; //一个队列独占..
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT; //双队列共享..
			createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilies.size());
			createInfo.pQueueFamilyIndices = queueFamilies.data();
		}


		//交换图片的图像变化，如：图片反转..
		createInfo.preTransform = swapChainSupportInfo.mCapabilities.currentTransform;

		//不与原来窗体中的内容混合..
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		createInfo.presentMode = presentMode;

		//窗体被遮挡的部分，不用绘制，但会影响回读..
		createInfo.clipped = VK_TRUE;

		if (vkCreateSwapchainKHR(mDevice->getDevice(), &createInfo, nullptr, &mSwapChain) != VK_SUCCESS) {
			std::runtime_error("Error: failed to create swapChain.");
		}

		mSwapChainFormat = surfaceFormat.format;
		mSwapChainExtent2D = extent;

		//系统可能有更多的image，当前 mImageCount 数量是最少的...
		vkGetSwapchainImagesKHR(mDevice->getDevice(), mSwapChain, &mImageCount, nullptr);

		mSwapChainImages.resize(mImageCount);
		vkGetSwapchainImagesKHR(mDevice->getDevice(), mSwapChain, &mImageCount, mSwapChainImages.data());

		//创建 ImageView..
		mSwapChainImageViews.resize(mImageCount);
		for (int i = 0; i < mImageCount; ++i) {
			mSwapChainImageViews[i] = createImageView(mSwapChainImages[i], mSwapChainFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
		}
	}

	swapChain::~swapChain() {
		for (int i = 0; i < mSwapChainImageViews.size(); ++i) {
			vkDestroyImageView(mDevice->getDevice(), mSwapChainImageViews[i], nullptr);
		}

		if (mSwapChain != VK_NULL_HANDLE) {
			vkDestroySwapchainKHR(mDevice->getDevice(), mSwapChain, nullptr);
		}

		for (int i = 0; i < mSwapChainFrameBuffers.size(); ++i) {
			vkDestroyFramebuffer(mDevice->getDevice(), mSwapChainFrameBuffers[i], nullptr);
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

	void swapChain::createFrameBuffers(const RenderPass::Ptr& renderpass)
	{
		mSwapChainFrameBuffers.resize(mImageCount);
		for (int i = 0; i < mImageCount; ++i) {
			std::array<VkImageView, 1> attachments = { mSwapChainImageViews[i] };

			VkFramebufferCreateInfo frameBufferCreateInfo{};
			frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			frameBufferCreateInfo.renderPass = renderpass->getRenderPass();
			frameBufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			frameBufferCreateInfo.pAttachments = attachments.data();
			frameBufferCreateInfo.width = mSwapChainExtent2D.width;
			frameBufferCreateInfo.height = mSwapChainExtent2D.height;
			frameBufferCreateInfo.layers = 1;

			if (vkCreateFramebuffer(mDevice->getDevice(), &frameBufferCreateInfo, nullptr, &mSwapChainFrameBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("Error: failed create to framebuffer.");
			}
		}
	
	}


	VkImageView swapChain::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevelsCount)
	{
		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;

		viewInfo.subresourceRange.aspectMask = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = mipLevelsCount;
		viewInfo.subresourceRange.layerCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;

		VkImageView imageView{ VK_NULL_HANDLE };
		if (vkCreateImageView(mDevice->getDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
			throw std::runtime_error("Error: failed to craete ImageVeiw.");
		}

		return imageView;
	}
}