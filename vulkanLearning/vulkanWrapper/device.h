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

		// ���������豸���..
		int rateDevice(VkPhysicalDevice deivce); 

		//����豸�Ƿ����ϵͳ������...
		bool isDeviceSuitable(VkPhysicalDevice device);

		//Ѱ�� QueueuFamily..
		void initQueueFamilies(VkPhysicalDevice device);

		// �߼�����
		void createLogicalDevice();

	private:

		//�����Կ�...
		VkPhysicalDevice mPhysicalDevice{ VK_NULL_HANDLE };
		Instance::Inst_Ptr mInstance{ nullptr };

		//�洢��ǰ��Ⱦ�������ID...
		std::optional<uint32_t> mGraphicQueueFamily;
		VkQueue mGraphicQueue{ VK_NULL_HANDLE };

		//�߼��豸
		VkDevice mDevice{ VK_NULL_HANDLE };
	};
};