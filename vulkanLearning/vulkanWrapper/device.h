#pragma once


#include "../base.h"
#include "instance.h"
#include "WindowSurface.h"

namespace FF::Wrapper {

	const std::vector<const char*> deviceRequiredExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	class Device {

	public:
		using Dev_Ptr = std::shared_ptr<Device>;
		static Dev_Ptr create(Instance::Inst_Ptr instance, WindowSurface::Ptr windowSurface) { 
			return std::make_shared<Device>(instance, windowSurface); 
		}

		Device(Instance::Inst_Ptr instance, WindowSurface::Ptr windowSurface);

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

		bool isQueueFamilyCompliete();

		[[nodiscard]] auto getDevice() const { return mDevice; }
		[[nodiscard]] auto getPhysicalDevice() const{ return mPhysicalDevice; }

	private:

		//�����Կ�...
		VkPhysicalDevice mPhysicalDevice{ VK_NULL_HANDLE };
		Instance::Inst_Ptr mInstance{ nullptr };

		//�洢��ǰ��Ⱦ�������ID...
		std::optional<uint32_t> mGraphicQueueFamily;
		VkQueue mGraphicQueue{ VK_NULL_HANDLE };

		//������ʾ��..
		std::optional<uint32_t> mPresentQueueFamily;
		VkQueue mPresentQueue{ VK_NULL_HANDLE };

		//�߼��豸
		VkDevice mDevice{ VK_NULL_HANDLE };


		WindowSurface::Ptr mWindowSurface{ nullptr };
	};
};