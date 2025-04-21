#pragma once
#ifndef _SPECTRAFRAME_RENDER_SUBPASS_MANAGER_
#define _SPECTRAFRAME_RENDER_SUBPASS_MANAGER_

#include "../Core/sfVkCommon.h"
#include <set>
#include <unordered_set>
#include <optional>
#include <functional>

namespace sf {
	typedef void(PFN_ExecuteRenderSubpass)(vkw::CommandBuffer&);

	struct RenderSubpass {
		std::string name;
		uint32_t dstSubpass;

		std::unordered_set<std::string> subpassInputs;
		std::unordered_set<std::string> subpassOutputs;
		std::optional<std::string> subpassDepth;

		bool writeDepth;

		std::function<PFN_ExecuteRenderSubpass> execute;
	};

	class RenderPass {
	private:
		struct RenderPassAttachment {
			std::string name;

			VkAttachmentDescription2 attachment;
		};

		struct SubpassBuildInfo {
			std::string name;

			std::vector<VkAttachmentReference2> inputRefs;
			std::vector<VkAttachmentReference2> outputRefs;
			std::optional<VkAttachmentReference2> depthRef;

			VkSubpassDependency2 dependency;
			VkSubpassDescription2 description;

			std::function<PFN_ExecuteRenderSubpass> execute;
		};

		std::vector<RenderSubpass> mSubpasses;

		std::vector<SubpassBuildInfo> mBuildSubpasses;
		std::unordered_map<std::string, SubpassBuildInfo*> mBuildSubpassesMap;

		std::set<RenderPassAttachment> mAttachments;

		VkRenderPass mRenderPass;

	public:
		~RenderPass();

		//RenderPass& addSubpass(const RenderSubpass& subpassDesc);
		//
		//RenderPass& build();
		//
		//RenderPass& execute(vkw::CommandBuffer& commandBuffer);

		void destroy();
	};
}

#endif