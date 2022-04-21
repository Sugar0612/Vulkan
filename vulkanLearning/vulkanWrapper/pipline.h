#pragma once
#include "../base.h"
#include "device.h"
#include "Shader.h"

namespace FF::Wrapper {
	class Pipline {

	public:

		using Ptr = std::shared_ptr<Pipline>;
		static Ptr create(Device::Dev_Ptr& device) { return std::make_shared<Pipline>(device); }

		Pipline(Device::Dev_Ptr& device);

		~Pipline();

		void setShaderGroup(std::vector<Shader::Ptr>& shaderGroup);

		void setViewports(const std::vector<VkViewport>& viewports) { mViewPorts = viewports; }
		void setScissors(const std::vector<VkRect2D>& scissors) { mScissors = scissors; }

		void build();

		void pushPipelineAttachment(const VkPipelineColorBlendAttachmentState& blendAttachment) {
			mBlendAttachmentStates.push_back(blendAttachment);
		}

	public:

		VkPipelineVertexInputStateCreateInfo mVertexInputState{};
		VkPipelineInputAssemblyStateCreateInfo mAssemblyState{};
		VkPipelineViewportStateCreateInfo mViewportState{};
		VkPipelineRasterizationStateCreateInfo mRasterState{};
		VkPipelineMultisampleStateCreateInfo mSampleState{};
		std::vector<VkPipelineColorBlendAttachmentState> mBlendAttachmentStates{};
		VkPipelineColorBlendStateCreateInfo mBlendState{};
		VkPipelineDepthStencilStateCreateInfo mDepthState{};
		VkPipelineLayoutCreateInfo mLayoutState{};

		//todo: RenderPass..

	private:

		VkPipeline mPipline{ VK_NULL_HANDLE };
		VkPipelineLayout mLayout{ VK_NULL_HANDLE };
		Device::Dev_Ptr mDevice{ nullptr };
		std::vector<Shader::Ptr> mShaders{};

		std::vector<VkViewport> mViewPorts;
		std::vector<VkRect2D> mScissors;
	};
}