#pragma once
#include "../base.h"
#include "device.h"

namespace FF::Wrapper {

	class Shader {
		
	public:

		using Ptr = std::shared_ptr<Shader>;
		static Ptr create(const Device::Dev_Ptr& device, const std::string fileName, VkShaderStageFlagBits shaderStage, std::string entryPoint)
		{ 
			return std::make_shared<Shader>(device, fileName, shaderStage, entryPoint);
		}

		Shader(const Device::Dev_Ptr& device, const std::string fileName, VkShaderStageFlagBits shaderStage, std::string entryPoint);

		~Shader();

		[[nodiscard]] auto getShaderModule() const { return mShaderModule; }
		[[nodiscard]] auto &getEntryPoint() const { return mEntryPoint; }
		[[nodiscard]] auto getShaderStage() const { return mShaderStage; }

	private:

		VkShaderModule mShaderModule{ VK_NULL_HANDLE };
		Device::Dev_Ptr mDevice{ nullptr };
		std::string mEntryPoint;
		VkShaderStageFlagBits mShaderStage;
	};
}