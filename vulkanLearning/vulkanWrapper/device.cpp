#include "device.h"

namespace FF::Wrapper {
	Device::Device(Instance::Inst_Ptr instance, WindowSurface::Ptr windowSurface) {
		mInstance = instance;
		mWindowSurface = windowSurface;

		pickDevicePhysical();
		initQueueFamilies(mPhysicalDevice);
		createLogicalDevice();
	}

	Device::~Device() {
		//vkDestroyDevice(mDevice, nullptr);
		mInstance.reset();
		mWindowSurface.reset();
	}

	void Device::pickDevicePhysical() {
		uint32_t deviceCount = 0;
		//获得 设备Count..
		vkEnumeratePhysicalDevices(mInstance->getInstance(), &deviceCount, nullptr);

		if (deviceCount == 0) {
			throw std::runtime_error("Error: faild get Device error!");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		//Get Physical Device...
		vkEnumeratePhysicalDevices(mInstance->getInstance(), &deviceCount, devices.data());

		//打分..
		std::multimap<int, VkPhysicalDevice> candidates;
		for (const auto& device : devices) {
			int score = rateDevice(device);
			candidates.insert(std::make_pair(score, device));
		}

		if (candidates.rbegin()->first > 0 && isDeviceSuitable(candidates.rbegin()->second)) {
			mPhysicalDevice = candidates.rbegin()->second;
		}
		
		if (mPhysicalDevice == VK_NULL_HANDLE) {
			throw std::runtime_error("Error: failed to Pick Physical Device..");
		}

	}

	int Device::rateDevice(VkPhysicalDevice device) {
		int score = 0;

		// device properties..
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);

		//Get Features of Device..
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		//如果是独立显卡那么...可以加分..
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			score += 1000;
		}

		//支持纹理..
		score += deviceProperties.limits.maxImageDimension2D;


		// 设备必须要支持 几何Shader...
		if (!deviceFeatures.geometryShader) {
			return 0;
		}

		return score;
	}

	// 必须是独立显卡..
	bool Device::isDeviceSuitable(VkPhysicalDevice device) {
		// device properties..
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);

		//Get Features of Device..
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
			   deviceFeatures.geometryShader;
	}

	void Device::initQueueFamilies(VkPhysicalDevice device) {
		uint32_t queueFamiliesCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamiliesCount, nullptr);

		// Get QueueFamilies...
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamiliesCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamiliesCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			//必须是有管线 而且 可以实施渲染任务的队列...
			if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
				mGraphicQueueFamily = i;
			}

			VkBool32 presentSupport = VK_FALSE;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, mWindowSurface->getWindowSurface(), &presentSupport);
			
			if (presentSupport) {
				mPresentQueueFamily = i;
			}

			//找到具有图形功能的队列，那么退出遍历...
			if (isQueueFamilyCompliete()) {
				break;
			}
			++i;
		}
	}

	void Device::createLogicalDevice() {
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> queueFamilies{ mGraphicQueueFamily.value(), mPresentQueueFamily.value() };

		for (auto queueFamily : queueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;

			//优先级...
			float queuePriority = 1.0;
			queueCreateInfo.pQueuePriorities = &queuePriority;

			queueCreateInfos.push_back(queueCreateInfo);
		}

		

		//填写逻辑信息
		VkPhysicalDeviceFeatures deviceFeatures = {};
		
		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
		deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceRequiredExtensions.size());
		deviceCreateInfo.ppEnabledExtensionNames = deviceRequiredExtensions.data();

		//Layer..
		if (mInstance->getbEnableValidationLayer()) {
			deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			deviceCreateInfo.enabledLayerCount = 0;
		}
		

		if (vkCreateDevice(mPhysicalDevice, &deviceCreateInfo, nullptr, &mDevice) != VK_SUCCESS) {
			throw std::runtime_error("Error: failed Create Device.");
		}

		//Create Queue..
		vkGetDeviceQueue(mDevice, mGraphicQueueFamily.value(), 0, &mGraphicQueue);
		vkGetDeviceQueue(mDevice, mPresentQueueFamily.value(), 0, &mPresentQueue);
	}

	bool Device::isQueueFamilyCompliete()
	{
		return mGraphicQueueFamily.has_value() && mPresentQueueFamily.has_value();
	}
};