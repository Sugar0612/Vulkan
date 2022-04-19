#include "Shader.h"

namespace FF::Wrapper {

	static std::vector<char> readBinary(const std::string& fileName) {
		std::ifstream file(fileName.c_str(), std::ios::ate | std::ios::binary | std::ios::in);

		if (!file) {
			throw std::runtime_error("Error: failed to open shader file.");
		}

		const size_t fileSize = file.tellg();
		//存..
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	Wrapper::Shader::Shader(const Device::Dev_Ptr& device, const std::string fileName, VkShaderStageFlagBits shaderStage, std::string entryPoint):
		mDevice(device), mShaderStage(shaderStage), mEntryPoint(entryPoint)
	{
		std::vector<char> codeBuffer = readBinary(fileName);

		VkShaderModuleCreateInfo shaderInfoCreate{};
		shaderInfoCreate.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderInfoCreate.codeSize = codeBuffer.size();
		shaderInfoCreate.pCode = reinterpret_cast<const uint32_t*>(codeBuffer.data()); // 重新解释内存空间..

		if (vkCreateShaderModule(mDevice->getDevice(), &shaderInfoCreate, nullptr, &mShaderModule) != VK_SUCCESS) {
			throw std::runtime_error("Error: Failed to create shader module.");
		}

	}

	Wrapper::Shader::~Shader()
	{
		if (mShaderModule != VK_NULL_HANDLE) {
			vkDestroyShaderModule(mDevice->getDevice(), mShaderModule, nullptr);
		}

	
	}
}