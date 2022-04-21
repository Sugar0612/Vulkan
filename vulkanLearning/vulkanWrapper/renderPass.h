#pragma once

#include "../base.h"
#include "device.h"

namespace FF::Wrapper {

	// 思路:
	/* VkAttachmentDescription 描述一个颜色的深度的模板... 并且有很多..(VkAttachmentDescription1， VkAttachmentDescription2， VkAttachmentDescription3...)
	   VkAttachmentReference 说明本个 SubPass 需要 attachment其中的一个... 数组的索引id是多少， 图片期望的格式是什么..
	   VkSubpass 就是一个子pass..
	   VkSubpassDependency 描述不同子模块的依赖关系..
	*/

	class SubPass {

	public:
		SubPass();

		~SubPass();

		[[nodiscard]] auto getSubPassDesription() const { return mSubPassDesription; }

	private:
		void addColorAttachmentReference(const VkAttachmentReference& ref);
		void addInputAttachmentReference(const VkAttachmentReference& ref);
		void setDepthStenciAttachmentReference(const VkAttachmentReference& ref);
		void buildSubPassDesription();

	private:
		VkSubpassDescription mSubPassDesription{};
		std::vector<VkAttachmentReference> mColorAttachmentReference{};
		std::vector<VkAttachmentReference> mInputAttachmentReference{};
		VkAttachmentReference mDepthStencilAttachmentReference{};
	};


	// 一个RenderPass 可以有很多 SubPass..
	class RenderPass {
		
	public:
		using Ptr = std::shared_ptr<RenderPass>;
		static Ptr create(Device::Dev_Ptr& device) { return std::make_shared<RenderPass>(device); }

		RenderPass(Device::Dev_Ptr& device);

		~RenderPass();

		void addSubPass(const SubPass& subPass);
		
		void addSubPassDependency(const VkSubpassDependency& dependency);

		void addAttachmentDescription(const VkAttachmentDescription& attachment);

		void buildRenderPass();
	private:
		VkRenderPass mRenderPass;
		std::vector<SubPass> mSubPasses;
		std::vector<VkSubpassDependency> mDependencies{};
		std::vector<VkAttachmentDescription> mAttachmentDescripitions{}; // 每个attachment的细节信息..

		Device::Dev_Ptr mDevice{ nullptr };
	};
}