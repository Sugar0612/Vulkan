#include"instance.h"

namespace FF::Wrapper {

	// ���� Layers..
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

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
			instCreateInfo.ppEnabledExtensionNames = validationLayers.data();
		}
		else {
			instCreateInfo.enabledLayerCount = 0;
		}

		if (vkCreateInstance(&instCreateInfo, nullptr, &mInstance) != VK_SUCCESS) {
			throw std::runtime_error("Error: failed create InstanceInfo!");
		}
	}

	Instance::~Instance() {
		vkDestroyInstance(mInstance, nullptr);
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
};