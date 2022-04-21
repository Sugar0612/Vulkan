#pragma once

#include "../base.h"
#include "device.h"

namespace FF::Wrapper {

	// ˼·:
	/* VkAttachmentDescription ����һ����ɫ����ȵ�ģ��... �����кܶ�..(VkAttachmentDescription1�� VkAttachmentDescription2�� VkAttachmentDescription3...)
	   VkAttachmentReference ˵������ SubPass ��Ҫ attachment���е�һ��... ���������id�Ƕ��٣� ͼƬ�����ĸ�ʽ��ʲô..
	   VkSubpass ����һ����pass..
	   VkSubpassDependency ������ͬ��ģ���������ϵ..
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


	// һ��RenderPass �����кܶ� SubPass..
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
		std::vector<VkAttachmentDescription> mAttachmentDescripitions{}; // ÿ��attachment��ϸ����Ϣ..

		Device::Dev_Ptr mDevice{ nullptr };
	};
}