#include "pipline.h"

namespace FF::Wrapper {
	Pipline::Pipline(Device::Dev_Ptr& device):
		mDevice(device)
	{
		
	}


	Pipline::~Pipline()
	{
		if (mLayout != VK_NULL_HANDLE) {
			vkDestroyPipelineLayout(mDevice->getDevice(), mLayout, nullptr);
		}

		if (mPipline != VK_NULL_HANDLE) {
			vkDestroyPipeline(mDevice->getDevice(), mPipline, nullptr);
		}
	}

	void Pipline::setShaderGroup(std::vector<Shader::Ptr>& shaderGroup)
	{
		mShaders = shaderGroup;
	}


	void Pipline::build()
	{
		//给每个shader 添加描述符...
		std::vector<VkPipelineShaderStageCreateInfo> shaderCreateInfos{};
		for (const auto& shader : mShaders) {
			VkPipelineShaderStageCreateInfo shaderCreateInfo{};

			shaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderCreateInfo.stage = shader->getShaderStage();
			shaderCreateInfo.pName = shader->getEntryPoint().c_str();
			shaderCreateInfo.module = shader->getShaderModule();

			shaderCreateInfos.push_back(shaderCreateInfo);
		}
	}
}