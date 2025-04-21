#include "sfFramegraphImageManager.h"
#include "../Core/sfWindow.h"

std::optional<sf::FrameGraphImageUsageInfo> sf::FrameGraphImageResource::findPreviousImageUsage(const uint32_t currentPassIndex) {
	for (int i = static_cast<int>(usageInfo.size()) - 1; i >= 0; i--) {
		if (usageInfo[i].passIndex < currentPassIndex)
			return usageInfo[i];
	}

	return std::nullopt;
}

sf::FrameGraphImageManager::~FrameGraphImageManager() {
	destroyAll();
}

sf::FrameGraphImageResource* sf::FrameGraphImageManager::getImage(const std::string name) {
	auto iter = mImages.find(name);

	return iter != mImages.end() ? &iter->second : nullptr;
}

sf::FrameGraphImageManager& sf::FrameGraphImageManager::addImage(const FrameGraphImageDescription& description) {
	mImages[description.name] = { description };

	return *this;
}

sf::FrameGraphImageManager& sf::FrameGraphImageManager::createImages() {
	for (auto& [name, res] : mImages) {
		if (res.description.external) continue;

		res.image
			.setCorePtr(&Window::getVk())
			.usage(res.description.usageImageType | res.description.extraImageUsage)
			.format(res.description.format)
			.extent(VkExtent3D{ res.description.extent.width, res.description.extent.height, 1 })
			.properties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
			.create();

		res.imageView
			.setCorePtr(res.image.getCorePtr())
			.format(res.description.format)
			.subresource(res.description.usageImageType & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT)
			.create(res.image.getImage());
	}

	return *this;
}

void sf::FrameGraphImageManager::destroyAll() {
	for (auto& [_, image] : mImages) {
		image.image.destroy();
		image.imageView.destroy();
		image.usageInfo.clear();
	}

	mImages.clear();
}