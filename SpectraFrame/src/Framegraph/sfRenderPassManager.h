#pragma once
#ifndef _SPECTRAFRAME_RENDER_PASS_MANAGER_
#define _SPECTRAFRAME_RENDER_PASS_MANAGER_

#include "../Core/sfVkCommon.h"
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

	struct RenderPassResource {
		std::unordered_set<std::string> inputs;
		std::unordered_set<std::string> outputs;
		std::optional<std::string> depth;
	};

	struct RenderPassDescription {
		std::string name;

		RenderPassResource resource;

		std::vector<RenderSubpass> subpasses;
	};

	class RenderPassManager {
	private:
		struct RenderPassAttachment {
			std::string name;

			VkAttachmentDescription2 attachment;
		};

		struct SubpassBuildInfo {
			std::vector<VkAttachmentReference2> inputRefs;
			std::vector<VkAttachmentReference2> outputRefs;
			std::optional<VkAttachmentReference2> depthRef;

			VkSubpassDependency2 dependency;
			VkSubpassDescription2 description;

			std::function<PFN_ExecuteRenderSubpass> execute;
		};

		struct RenderPassBuildInfo {
			VkRenderPass renderPass;

			std::vector<RenderPassAttachment> attachments;
			std::unordered_map<std::string, RenderPassAttachment*> attachmentLookupMap;

			// Sorted based on dst subpass
			std::vector<SubpassBuildInfo> subpassInfo;
		};

		std::vector<RenderPassDescription> mDescriptions;
		std::unordered_map<std::string, RenderPassDescription*> mDescriptionLookupMap;

		std::vector<RenderPassBuildInfo> mBuild;
		std::unordered_map<std::string, RenderPassBuildInfo*> mBuildLookupMap;

	public:
		~RenderPassManager();

		RenderPassManager& addPass(const std::string& name);
		
		RenderPassManager& addSubpass(const std::string& renderPassName, const RenderSubpass& subpassDesc);
		
		RenderPassManager& build();
		
		RenderPassManager& execute(vkw::CommandBuffer& commandBuffer);

		void destroy();
	};
}

#endif