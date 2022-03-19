#pragma once

#include"../base.h"
#include <vector>

namespace FF::Wrapper {
	class Instance {

	public:

		// sharded_ptr<template> bind make_shared<template>..
		using Inst_Ptr = std::shared_ptr<Instance>;

		// return type shared_ptr on use make_shared..
		static Inst_Ptr create() { return std::make_shared<Instance>(); }

		Instance();

		~Instance();

	public:

		// ��ӡVulkan�������е���չ��..
		void printAvailableExtensions();


		std::vector<const char*> getRequiredExtensions();
	private:

		VkInstance mInstance;
	};
};