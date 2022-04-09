#pragma once


#include "../base.h"
#include "instance.h"
#include <map>

namespace FF::Wrapper {
	class Device {

	public:
		using Dev_Ptr = std::shared_ptr<Device>;
		static Dev_Ptr create(Instance::Inst_Ptr instance) { return std::make_shared<Device>(instance); }

		Device(Instance::Inst_Ptr instance);

		~Device();

		void pickDevicePhysical();

		// 基于物理设备打分..
		int retaDevice(VkPhysicalDevice deivce); 

		//这个设备是否符合系统的需求...
		bool isDeviceSuitable(VkPhysicalDevice device);
	private:

		//物理显卡...
		VkPhysicalDevice mPhysicalDevice{ VK_NULL_HANDLE };
		Instance::Inst_Ptr mInstance;
	};
};