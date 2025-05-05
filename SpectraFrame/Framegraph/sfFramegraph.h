#pragma once
#ifndef _SPECTRAFRAME_FRAMEGRAPH_
#define _SPECTRAFRAME_FRAMEGRAPH_

#include "sfFramegraphImageManager.h"
#include "sfResourceManager.h"

namespace sf {
	struct FrameGraphPass {
		std::string passName;

		std::unordered_set<std::string> passResources;
		std::unordered_set<std::string> passReads;
		std::unordered_set<std::string> passWrites;
		std::optional<std::string> passDepth;
		std::function<void(vkw::CommandBuffer&, const std::unordered_map<std::string, FrameGraphImageResource*>&)> passExecute;

		FrameGraphPass& reads(const std::initializer_list<std::string>& reads);

		FrameGraphPass& writes(const std::initializer_list<std::string>& writes);

		FrameGraphPass& depth(std::string depth);

		FrameGraphPass& executes(std::function<void(vkw::CommandBuffer&, const std::unordered_map<std::string, FrameGraphImageResource*>&)> executeFunction);
	};

	class FrameGraph {
	private:
		struct BuiltPass {
			vkw::RenderPass renderPass;

			std::vector<FrameGraphImageResource*> passFramebufferImages;
			std::unordered_map<std::string, FrameGraphImageResource*> passImageResources;

			std::vector<VkClearValue> renderPassClearValues;

			std::function<void(vkw::CommandBuffer&, const std::unordered_map<std::string, FrameGraphImageResource*>&)> passExecuteCopy;

			void recordBarriers(vkw::CommandBuffer& commandBuffer, const uint32_t currentPassIndex);
		};

		std::vector<FrameGraphPass> mPasses;
		std::vector<FrameGraphPass*> mSortedPasses;

		std::vector<BuiltPass> mBuiltPasses;

		FrameGraphImageManager mImageManager;
		ResourceManager mResourceManager;

		std::vector<FrameGraphImageResource*> _getRequiredWriteImages(const FrameGraphPass& desiredPass);

		void _swapPasses(FrameGraphPass* pFirst, FrameGraphPass* pSecond);

		void _sortPasses();

	public:
		~FrameGraph();

		std::vector<FrameGraphPass>& getPasses();

		/**
		 * @brief Adds new pass with name, keep in mind correct order as for now there is no passes sorting 
		 * @param passName 
		 * @return 
		 */
		FrameGraphPass& addPass(const std::string& passName);

		/**
		 * @brief Adds image that gonna be input/output for next render passes, passing images should be handled by user for now
		 * @param description 
		 * @return 
		 */
		FrameGraph& addImage(const FrameGraphImageDescription& description);

		FrameGraph& importSwapchainImage(const std::string& imageName, vkw::Image& image, vkw::ImageView& imageView);

		FrameGraph& clearPasses();

		//vkw::RenderPass* editPassSubpasses(const std::string& passName);

		FrameGraph& buildGraph();

		FrameGraph& executeGraph(vkw::CommandBuffer& commandBuffer);

		void destroy();
	};
}

#endif