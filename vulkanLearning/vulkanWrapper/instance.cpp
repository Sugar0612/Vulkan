#include"instance.h"

namespace FF::Wrapper {

	Instance::Instance() {
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Vulkan";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "NO ENGINE";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo instCreateInfo = {};
		instCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instCreateInfo.pApplicationInfo = &appInfo;

		if (vkCreateInstance(&instCreateInfo, nullptr, &mInstance) != VK_SUCCESS) {
			std::cout << "Error: failed create InstanceInfo!" << std::endl;
		}
	}

	Instance::~Instance() {
		vkDestroyInstance(mInstance, nullptr);
	}
};