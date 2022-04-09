#pragma once

#include"../base.h"
#include <vector>
#include <string>

namespace FF::Wrapper {
	class Instance {

	public:

		// sharded_ptr<template> bind make_shared<template>..
		using Inst_Ptr = std::shared_ptr<Instance>;

		// return type shared_ptr on use make_shared..
		static Inst_Ptr create(bool enableValidationLayer) { return std::make_shared<Instance>(enableValidationLayer); }

		Instance(bool enableVaildationLayer);

		~Instance();

	public:

		// 打印Vulkan里面所有的扩展名..
		void printAvailableExtensions();


		std::vector<const char*> getRequiredExtensions();

		// vaildationLayers..
		bool checkVaildationLayersSupport();
		void setupDebugger();

		[[nodiscard]] VkInstance getInstance() const { return mInstance; }

	private:

		VkInstance mInstance{VK_NULL_HANDLE};
		bool bEnableValidationLayer = false;
		VkDebugUtilsMessengerEXT mDebugger;
	};
};