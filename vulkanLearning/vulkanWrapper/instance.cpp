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

		VkInstanceCreateInfo instCreateInfo = {};  //你也可以开启你想开启的扩展..
		instCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instCreateInfo.pApplicationInfo = &appInfo;

		std::vector<const char*> extensions = getRequiredExtensions();
		instCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		instCreateInfo.ppEnabledExtensionNames = extensions.data();
		if (vkCreateInstance(&instCreateInfo, nullptr, &mInstance) != VK_SUCCESS) {
			std::cout << "Error: failed create InstanceInfo!" << std::endl;
		}
	}

	Instance::~Instance() {
		vkDestroyInstance(mInstance, nullptr);
	}

	void Instance::printAvailableExtensions() {
		uint32_t extensionsCount = 0;

		//可以获得系统中有多少个扩展，并且没有扩展是什么，并且放在一个array中..
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);
		std::vector<VkExtensionProperties> vc_extensions(extensionsCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, vc_extensions.data());

		for (const auto ex : vc_extensions) {
			std::cout << ex.extensionName << ": " << ex.specVersion << std::endl;
		}
	}

	std::vector<const char*> Instance::getRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		
		// 该函数可以一方面把扩展名给予 glfwExtensions，一方面可以把需要多少个传给 glfwExtensionCount..
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		// begin() ~ end()..
		std::vector<const char*> Extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		return Extensions;
	}
};