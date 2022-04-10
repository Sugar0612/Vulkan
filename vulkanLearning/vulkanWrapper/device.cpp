#include "device.h"

namespace FF::Wrapper {
	Device::Device(Instance::Inst_Ptr instance) {
		mInstance = instance;
		pickDevicePhysical();
		initQueueFamilies(mPhysicalDevice);
		createLogicalDevice();
	}

	Device::~Device() {
		//vkDestroyDevice(mDevice, nullptr);
		mInstance.reset();
	}

	void Device::pickDevicePhysical() {
		uint32_t deviceCount = 0;
		//��� �豸Count..
		vkEnumeratePhysicalDevices(mInstance->getInstance(), &deviceCount, nullptr);

		if (deviceCount == 0) {
			throw std::runtime_error("Error: faild get Device error!");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		//Get Physical Device...
		vkEnumeratePhysicalDevices(mInstance->getInstance(), &deviceCount, devices.data());

		//���..
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

		//����Ƕ����Կ���ô...���Լӷ�..
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			score += 1000;
		}

		//֧������..
		score += deviceProperties.limits.maxImageDimension2D;


		// �豸����Ҫ֧�� ����Shader...
		if (!deviceFeatures.geometryShader) {
			return 0;
		}

		return score;
	}

	// �����Ƕ����Կ�..
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
			//�������й��� ���� ����ʵʩ��Ⱦ����Ķ���...
			if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
				mGraphicQueueFamily = i;
			}

			//�ҵ�����ͼ�ι��ܵĶ��У���ô�˳�����...
			if (mGraphicQueueFamily.has_value()) {
				break;
			}
			++i;
		}
	}

	void Device::createLogicalDevice() {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = mGraphicQueueFamily.value();
		queueCreateInfo.queueCount = 1;

		//���ȼ�...
		float queuePriority = 1.0;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		//��д�߼���Ϣ
		VkPhysicalDeviceFeatures deviceFeatures = {};
		
		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
		deviceCreateInfo.queueCreateInfoCount = 1;
		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
		deviceCreateInfo.enabledExtensionCount = 0;

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
	}
};