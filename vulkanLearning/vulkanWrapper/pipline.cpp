#include "pipline.h"

namespace FF::Wrapper {
	Pipline::Pipline(Device::Dev_Ptr& device):
		mDevice(device)
	{
		mVertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		mAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	    mViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		mRasterState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		mSampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		mBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		mDepthState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		mLayoutState.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
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
		//��ÿ��shader ���������...
		std::vector<VkPipelineShaderStageCreateInfo> shaderCreateInfos{};
		for (const auto& shader : mShaders) {
			VkPipelineShaderStageCreateInfo shaderCreateInfo{};

			shaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderCreateInfo.stage = shader->getShaderStage();
			shaderCreateInfo.pName = shader->getEntryPoint().c_str();
			shaderCreateInfo.module = shader->getShaderModule();

			shaderCreateInfos.push_back(shaderCreateInfo);
		}

		//�ӿڲü�...
		mViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		mViewportState.viewportCount = static_cast<uint32_t>(mViewPorts.size());
		mViewportState.pViewports = mViewPorts.data();
		mViewportState.scissorCount = static_cast<uint32_t>(mScissors.size());
		mViewportState.pScissors = mScissors.data();

		//blend..
		mBlendState.attachmentCount = static_cast<uint32_t>(mBlendAttachmentStates.size());
		mBlendState.pAttachments = mBlendAttachmentStates.data();

		//create to Layout..
		if (mLayout != VK_NULL_HANDLE) {
			vkDestroyPipelineLayout(mDevice->getDevice(), mLayout, nullptr);
		}

		if (vkCreatePipelineLayout(mDevice->getDevice(), &mLayoutState, nullptr, &mLayout) != VK_SUCCESS) {
			throw std::runtime_error("Error: failed create to layout.");
		}

		VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

		pipelineCreateInfo.stageCount = static_cast<uint32_t>(shaderCreateInfos.size());
		pipelineCreateInfo.pStages = shaderCreateInfos.data();

		pipelineCreateInfo.pVertexInputState = &mVertexInputState;
		pipelineCreateInfo.pInputAssemblyState = &mAssemblyState;
		pipelineCreateInfo.pViewportState = &mViewportState;
		pipelineCreateInfo.pRasterizationState = &mRasterState;
		pipelineCreateInfo.pMultisampleState = &mSampleState;
		pipelineCreateInfo.pColorBlendState = &mBlendState;
		pipelineCreateInfo.pDepthStencilState = nullptr; // todo: ..
		pipelineCreateInfo.layout = mLayout;
		
		pipelineCreateInfo.renderPass = VK_NULL_HANDLE; // todo: ..
		pipelineCreateInfo.subpass = 0;

		//�Ѵ��ڵ� pipelineΪ������д�������Ҫ flags Ϊ VK_PIPELINE_CREATE_DERIVATIVE_BIT
		pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineCreateInfo.basePipelineIndex = -1;

		if (mPipline != VK_NULL_HANDLE) {
			vkDestroyPipeline(mDevice->getDevice(), mPipline, nullptr);
		}

		//pipelineCache ���Խ�������ݴ��뻺�棬�ڶ��pipeline��ʹ�ã�Ҳ���Դ����ļ��ò�ͬ�������..
		if (vkCreateGraphicsPipelines(mDevice->getDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &mPipline) != VK_SUCCESS) {
			throw std::runtime_error("Error: failed create to pipeline.");
		}
	}
}