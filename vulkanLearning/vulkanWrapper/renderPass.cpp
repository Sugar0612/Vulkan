#include "renderPass.h"

namespace FF::Wrapper {

	
	
	SubPass::SubPass()
	{
	
	}

	SubPass::~SubPass()
	{
	
	}

	void SubPass::addColorAttachmentReference(const VkAttachmentReference& ref)
	{
		mColorAttachmentReference.push_back(ref);
	}

	void SubPass::addInputAttachmentReference(const VkAttachmentReference& ref)
	{
		mInputAttachmentReference.push_back(ref);
	}

	void SubPass::setDepthStenciAttachmentReference(const VkAttachmentReference& ref)
	{
		mDepthStencilAttachmentReference = ref;
	}

	void SubPass::buildSubPassDesription()
	{
		if (mColorAttachmentReference.empty()) {
			throw std::runtime_error("Error: color attachment reference is empty.");
		}
		mSubPassDesription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		mSubPassDesription.colorAttachmentCount = static_cast<uint32_t>(mColorAttachmentReference.size());
		mSubPassDesription.pColorAttachments = mColorAttachmentReference.data();

		mSubPassDesription.inputAttachmentCount = static_cast<uint32_t>(mInputAttachmentReference.size());
		mSubPassDesription.pInputAttachments = mInputAttachmentReference.data();

		mSubPassDesription.pDepthStencilAttachment = &mDepthStencilAttachmentReference;
	}

	RenderPass::RenderPass(Device::Dev_Ptr& device):
		mDevice(device)
	{
	
	}


	RenderPass::~RenderPass()
	{
		if (mRenderPass != nullptr) {
			vkDestroyRenderPass(mDevice->getDevice(), mRenderPass, nullptr);
		}
	}

	void RenderPass::addSubPass(const SubPass& subPass)
	{
		mSubPasses.push_back(subPass);
	}

	void RenderPass::addSubPassDependency(const VkSubpassDependency& dependency)
	{
		mDependencies.push_back(dependency);

	}

	void RenderPass::addAttachmentDescription(const VkAttachmentDescription& attachment)
	{
		mAttachmentDescripitions.push_back(attachment);
	}

	void RenderPass::buildRenderPass()
	{
		if (mSubPasses.empty() || mDependencies.empty() || mAttachmentDescripitions.empty()) {
			throw std::runtime_error("Error: not enough elements to build renderPass.");
		}

		// 我们需要获得每个Subpass的  DeDescription..
		std::vector<VkSubpassDescription> Subpasses{};
		for (int i = 0; i < mSubPasses.size(); ++i) {
			Subpasses.push_back(mSubPasses[i].getSubPassDesription());
		}

		VkRenderPassCreateInfo createInfo;
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

		createInfo.attachmentCount = static_cast<uint32_t>(mAttachmentDescripitions.size());
		createInfo.pAttachments = mAttachmentDescripitions.data();

		createInfo.dependencyCount = static_cast<uint32_t>(mDependencies.size());
		createInfo.pDependencies = mDependencies.data();

		createInfo.subpassCount = static_cast<uint32_t>(Subpasses.size());
		createInfo.pSubpasses = Subpasses.data();

		if(vkCreateRenderPass(mDevice->getDevice(), &createInfo, nullptr, &mRenderPass) != VK_SUCCESS) {
			throw std::runtime_error("Error: failed create to RenderPass.");
		}
	}

}
