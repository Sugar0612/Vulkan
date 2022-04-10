#pragma once


#include "../base.h"
#include "instance.h"

namespace FF::Wrapper {
	class Device {

	public:
		using Dev_Ptr = std::shared_ptr<Device>;
		static Dev_Ptr create(Instance::Inst_Ptr instance) { return std::make_shared<Device>(instance); }

		Device(Instance::Inst_Ptr instance);

		~Device();

		void pickDevicePhysical();

		// 基于物理设备打分..
		int rateDevice(VkPhysicalDevice deivce); 

		//这个设备是否符合系统的需求...
		bool isDeviceSuitable(VkPhysicalDevice device);

		//寻找 QueueuFamily..
		void initQueueFamilies(VkPhysicalDevice device);

		// 逻辑队列
		void createLogicalDevice();

	private:

		//物理显卡...
		VkPhysicalDevice mPhysicalDevice{ VK_NULL_HANDLE };
		Instance::Inst_Ptr mInstance{ nullptr };

		//存储当前渲染队列族的ID...
		std::optional<uint32_t> mGraphicQueueFamily;
		VkQueue mGraphicQueue{ VK_NULL_HANDLE };

		//逻辑设备
		VkDevice mDevice{ VK_NULL_HANDLE };
	};
};