#include"instance.h"

namespace FF::Wrapper {

	//vadlidation layer�ص�����
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallBack(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pMessageData,
	void *pUserData) {
		std::cout << "ValidationLayer: " << pMessageData->pMessage << std::endl;
		return VK_FALSE;
	}

	//Info Create ��������
	static VkResult CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks *pAllocator,
		VkInstance instance,
		VkDebugUtilsMessengerEXT *debugMessenger) {
		auto fun = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

		if (fun != nullptr) {
			return fun(instance, pCreateInfo, pAllocator, debugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	// Info Destroy ��������
	static void DestroyDebugUtilsMessagerEXT(VkInstance instance,
		const VkAllocationCallbacks *pAllocator, 
		VkDebugUtilsMessengerEXT debugMessenger) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

		if (func != nullptr) {
			return func(instance, debugMessenger, pAllocator);
		}
	}

	Instance::Instance(bool enableValidationLayer): bEnableValidationLayer(enableValidationLayer) {
		// �����õ�ǰ���¼��...
		if (bEnableValidationLayer && !checkVaildationLayersSupport()) {
			throw std::runtime_error("Error: vaildation Layer is not supported!");
		}

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Vulkan";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "NO ENGINE";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo instCreateInfo = {};  //��Ҳ���Կ������뿪������չ..
		instCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instCreateInfo.pApplicationInfo = &appInfo;

		std::vector<const char*> extensions = getRequiredExtensions();
		instCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		instCreateInfo.ppEnabledExtensionNames = extensions.data();

		// of layers..
		if (bEnableValidationLayer) {
			instCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			instCreateInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			instCreateInfo.enabledLayerCount = 0;
		}

		if (vkCreateInstance(&instCreateInfo, nullptr, &mInstance) != VK_SUCCESS) {
			throw std::runtime_error("Error: failed create InstanceInfo!");
		}

		setupDebugger();
	}

	void Instance::printAvailableExtensions() {
		uint32_t extensionsCount = 0;

		//���Ի��ϵͳ���ж��ٸ���չ������û����չ��ʲô�����ҷ���һ��array��..
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);
		std::vector<VkExtensionProperties> vc_extensions(extensionsCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, vc_extensions.data());

		for (const auto ex : vc_extensions) {
			std::cout << ex.extensionName << ": " << ex.specVersion << std::endl;
		}
	}

	std::vector<const char*> Instance::getRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		
		// �ú�������һ�������չ������ glfwExtensions��һ������԰���Ҫ���ٸ����� glfwExtensionCount..
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		// begin() ~ end()..
		std::vector<const char*> Extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		Extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		return Extensions;
	}

	bool Instance::checkVaildationLayersSupport() {
		uint32_t LayerCount = 0;
		vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);
		std::vector<VkLayerProperties> vc_Layers(LayerCount);
		vkEnumerateInstanceLayerProperties(&LayerCount, vc_Layers.data());

		for (auto& LayerName : validationLayers) {
			bool isFound = false;

			for (auto& validName : vc_Layers) {
				if (0 == std::strcmp(LayerName, validName.layerName)) {
					isFound = true;
					break;
				}
			}

			if (!isFound) {
				return false;
			}
		}

		return true;
	}

	void Instance::setupDebugger() {
		if (!bEnableValidationLayer) return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

		//��Ϣ����
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallBack;
		createInfo.pUserData = nullptr;

		if (CreateDebugUtilsMessengerEXT(&createInfo, nullptr, mInstance, &mDebugger) != VK_ERROR_EXTENSION_NOT_PRESENT) {
			std::runtime_error("Error: Failed to Create Debugger!");
		}
	}

	Instance::~Instance() {
		//ֻ���㿪���˲ſ�������...
		if (bEnableValidationLayer) {
			DestroyDebugUtilsMessagerEXT(mInstance, nullptr, mDebugger);
		}

		vkDestroyInstance(mInstance, nullptr);
	}
};