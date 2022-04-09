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
		//获得 设备Count..
		vkEnumeratePhysicalDevices(mInstance->getInstance(), &deviceCount, nullptr);

		if (deviceCount == 0) {
			throw std::runtime_error("Error: faild get Device error!");
			return;
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		//Get Physical Device...
		vkEnumeratePhysicalDevices(mInstance->getInstance(), &deviceCount, devices.data());

		//打分..
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
};