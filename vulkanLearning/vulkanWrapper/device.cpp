#include "device.h"

namespace FF::Wrapper {
	Device::Device(Instance::Inst_Ptr instance) {
		mInstance = instance;
		pickDevicePhysical();
	}

	Device::~Device() {
		mInstance.reset();
	}

	void Device::pickDevicePhysical() {
		uint32_t deviceCount = 0;
		//��� �豸Count..
		vkEnumeratePhysicalDevices(mInstance->getInstance(), &deviceCount, nullptr);

		if (deviceCount == 0) {
			throw std::runtime_error("Error: faild get Device error!");
			return;
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		//Get Physical Device...
		vkEnumeratePhysicalDevices(mInstance->getInstance(), &deviceCount, devices.data());

		//���..
		std::multimap<int, VkPhysicalDevice> candidates;
		for (auto device : devices) {
			int score = retaDevice(device);
			candidates.insert(std::make_pair(score, device));
		}

		if (candidates.rbegin()->first > 0 && isDeviceSuitable(candidates.rbegin()->second)) {
			mPhysicalDevice = candidates.rbegin()->second;
		}
		else {
			throw std::runtime_error("Error: faild get Device.");
			return;
		}

	}

	int Device::retaDevice(VkPhysicalDevice device) {
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
};