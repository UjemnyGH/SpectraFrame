#pragma once
#ifndef _SPECTRAFRAME_FRAME_GRAPH_IMAGE_MANAGER_
#define _SPECTRAFRAME_FRAME_GRAPH_IMAGE_MANAGER_

#include "../Core/sfVkCommon.h"
#include <functional>
#include <unordered_set>
#include <optional>

namespace sf {
	struct FrameGraphImageDescription {
		std::string name;
		VkFormat format;
		VkExtent2D extent;
		VkImageUsageFlags usageImageType;
		VkImageUsageFlags extraImageUsage;
		bool external;
		bool isSwapchainPart;
	};

	struct FrameGraphImageUsageInfo {
		uint32_t passIndex;
		VkPipelineStageFlags stage;
		VkAccessFlags access;
		VkImageLayout layout;
		bool isWrite;
		VkImageAspectFlags aspect;
	};

	struct FrameGraphImageResource {
		FrameGraphImageDescription description;
		std::vector<FrameGraphImageUsageInfo> usageInfo;

		vkw::Image image;
		vkw::ImageView imageView;

		std::optional<FrameGraphImageUsageInfo> findPreviousImageUsage(const uint32_t currentPassIndex);
	};

	class FrameGraphImageManager {
	private:
		std::unordered_map<std::string, FrameGraphImageResource> mImages;

	public:
		~FrameGraphImageManager();

		FrameGraphImageResource* getImage(const std::string name);

		FrameGraphImageManager& addImage(const FrameGraphImageDescription& description);

		FrameGraphImageManager& createImages();

		void destroyAll();
	};
}

#endif