#include "sfFramegraph.h"
#include "../Core/sfLogger.h"
#include "../Core/sfWindow.h"

sf::FrameGraphPass& sf::FrameGraphPass::reads(const std::initializer_list<std::string>& rd) {
	passReads.insert(rd);
	
	return *this;
}

sf::FrameGraphPass& sf::FrameGraphPass::writes(const std::initializer_list<std::string>& wr) {
	passWrites.insert(wr);
	
	return *this;
}

sf::FrameGraphPass& sf::FrameGraphPass::depth(std::string depth) {
	passDepth = depth;

	return *this;
}

sf::FrameGraphPass& sf::FrameGraphPass::executes(std::function<void(vkw::CommandBuffer&, const std::unordered_map<std::string, FrameGraphImageResource*>&)> executeFunction) {
	passExecute = executeFunction;

	return *this;
}

std::vector<sf::FrameGraphImageResource*> sf::FrameGraph::_getRequiredWriteImages(const FrameGraphPass& desiredPass) {
	std::vector<FrameGraphImageResource*> result;

	for (std::string name : desiredPass.passWrites) {
		result.push_back(mImageManager.getImage(name));
	}

	if (desiredPass.passDepth.has_value())
		result.push_back(mImageManager.getImage(desiredPass.passDepth.value()));

	return result;
}

void sf::FrameGraph::BuiltPass::recordBarriers(vkw::CommandBuffer& commandBuffer, const uint32_t currentPassIndex) {
	std::vector<VkImageMemoryBarrier2> barriers;

	for (const auto& [name, image] : passImageResources) {
		auto usageIter = std::find_if(image->usageInfo.begin(), image->usageInfo.end(), [&](const FrameGraphImageUsageInfo& u) { return u.passIndex == currentPassIndex; });

		if (usageIter == image->usageInfo.end()) continue;

		std::optional<FrameGraphImageUsageInfo> prev = image->findPreviousImageUsage(currentPassIndex);

		if (!prev.has_value()) continue;

		if (prev->layout != usageIter->layout || prev->access != usageIter->access || prev->stage != usageIter->stage) {
			VkImageMemoryBarrier2 barrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
			barrier.oldLayout = prev->layout;
			barrier.newLayout = usageIter->layout;
			barrier.srcAccessMask = prev->access;
			barrier.dstAccessMask = usageIter->access;
			barrier.srcStageMask = prev->stage;
			barrier.dstStageMask = usageIter->stage;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.image = image->image.getImage();
			barrier.subresourceRange = {
				.aspectMask = usageIter->aspect,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			};

			barriers.push_back(barrier);
		}
	}

	if (!barriers.empty()) {
		VkDependencyInfo transitionBarriersInfo = { VK_STRUCTURE_TYPE_DEPENDENCY_INFO };
		transitionBarriersInfo.imageMemoryBarrierCount = static_cast<uint32_t>(barriers.size());
		//transitionBarriersInfo.pImageMemoryBarriers = &barriers[0];

		commandBuffer.cmdPipelineBarrier2(&transitionBarriersInfo);
	}
		// Legacy code
		//commandBuffer.cmdPipelineBarrier2(VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, static_cast<uint32_t>(barriers.size()), &barriers[0]);
}

void sf::FrameGraph::_swapPasses(FrameGraphPass* pFirst, FrameGraphPass* pSecond) {
	bool swapped = false;

	if (pFirst->passReads.empty()) {
		FrameGraphPass* second = pSecond;
		pSecond = pFirst;
		pFirst = second;

		return;
	}

	for (std::string secondWrites : pSecond->passWrites) {
		for (std::string firstReads : pFirst->passReads) {
			if (secondWrites == firstReads) {
				FrameGraphPass* second = pSecond;
				pSecond = pFirst;
				pFirst = second;

				swapped = true;

				break;
			}
		}

		if (swapped) break;
	}
}

void sf::FrameGraph::_sortPasses() {
	mSortedPasses.clear();
	mSortedPasses.reserve(mPasses.size());
	
	for (size_t i = 0; i < mPasses.size(); i++) {
		mSortedPasses.push_back(&mPasses[i]);
	}

	for (size_t i = 0; i < mSortedPasses.size(); i++) {
		FrameGraphPass* passI = mSortedPasses[i];

		for (size_t j = i; j < mSortedPasses.size(); j++) {
			FrameGraphPass* passJ = mSortedPasses[j];

			if (passI == passJ) continue;

			_swapPasses(passI, passJ);
		}
	}
}

sf::FrameGraph::~FrameGraph() {
	destroy();
}

std::vector<sf::FrameGraphPass>& sf::FrameGraph::getPasses() {
	return mPasses;
}

sf::FrameGraphPass& sf::FrameGraph::addPass(const std::string& passName) {
	FrameGraphPass pass;
	pass.passName = passName;
	
	mPasses.emplace_back(std::move(pass));

	return *(mPasses.end() - 1);
}

sf::FrameGraph& sf::FrameGraph::addImage(const FrameGraphImageDescription& description) {
	mImageManager.addImage(description);

	return *this;
}

sf::FrameGraph& sf::FrameGraph::importSwapchainImage(const std::string& imageName, vkw::Image& image, vkw::ImageView& imageView) {
	mImageManager.addImage(FrameGraphImageDescription{
		.name = imageName,
		.external = true,
		.isSwapchainPart = true
	});

	mImageManager.getImage(imageName)->image = image;
	mImageManager.getImage(imageName)->imageView = imageView;

	return *this;
}

sf::FrameGraph& sf::FrameGraph::clearPasses() {
	destroy();

	mPasses.clear();

	return *this;
}

// TODO: Add support for supbasses in FrameGraph
/*vkw::RenderPass* sf::FrameGraph::editPassSubpasses(const std::string& passName) {
	for (FrameGraphPass& pass : mPasses) {
		if (pass.passName == passName)
			return &pass.renderPass;
	}

	return nullptr;
}*/

sf::FrameGraph& sf::FrameGraph::buildGraph() {
	mImageManager.createImages();

	_sortPasses();

	for (size_t passIndex = 0; passIndex < mSortedPasses.size(); passIndex++) {
		const FrameGraphPass& pass = *mSortedPasses[passIndex];
		
		std::unordered_map<std::string, FrameGraphImageResource*> imageResources;
		std::vector<VkClearValue> clearValues;

		mBuiltPasses.push_back(BuiltPass{
			//.renderPass = renderPass,
			.passFramebufferImages = _getRequiredWriteImages(pass),
			//.passImageResources = imageResources,
			//.renderPassClearValues = clearValues,
			.passExecuteCopy = pass.passExecute
			});

		//vkw::RenderPass renderPass;
		(mBuiltPasses.end() - 1)->renderPass
			.setCorePtr(&Window::getVk())
			.addSubpass(pass.passDepth.has_value());

		for (const std::string& readName : pass.passReads) {
			FrameGraphImageResource* pRes = mImageManager.getImage(readName);
			imageResources[readName] = pRes;

			if (pRes->description.isSwapchainPart) {
				pRes->usageInfo.emplace_back(FrameGraphImageUsageInfo{
					.passIndex = static_cast<uint32_t>(passIndex),
					.stage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
					.access = 0,
					.layout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
					.isWrite = true,
					.aspect = VK_IMAGE_ASPECT_COLOR_BIT
				});
			}
			else {
				pRes->usageInfo.emplace_back(FrameGraphImageUsageInfo{
					.passIndex = static_cast<uint32_t>(passIndex),
					.stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
					.access = VK_ACCESS_SHADER_READ_BIT,
					.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
					.isWrite = false,
					.aspect = VK_IMAGE_ASPECT_COLOR_BIT
				});
			}
		}

		for (const std::string& writeName : pass.passWrites) {
			FrameGraphImageResource* pRes = mImageManager.getImage(writeName);
			imageResources[writeName] = pRes;

			if (pRes->description.isSwapchainPart) {
				pRes->usageInfo.emplace_back(FrameGraphImageUsageInfo{
					.passIndex = static_cast<uint32_t>(passIndex),
					.stage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
					.access = 0,
					.layout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
					.isWrite = true,
					.aspect = VK_IMAGE_ASPECT_COLOR_BIT
				});
			}
			else {
				pRes->usageInfo.emplace_back(FrameGraphImageUsageInfo{
					.passIndex = static_cast<uint32_t>(passIndex),
					.stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
					.access = VK_ACCESS_SHADER_WRITE_BIT,
					.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
					.isWrite = true,
					.aspect = VK_IMAGE_ASPECT_COLOR_BIT
				});
			}

			(mBuiltPasses.end() - 1)->renderPass
				.addAttachment(writeName, pRes->description.format, VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
				.addSubpassDependency(0, VK_SUBPASS_EXTERNAL, 0, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, 0, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT)
				.subpassAddOutputRef(0, (mBuiltPasses.end() - 1)->renderPass.getAttachmentId(writeName), VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT);

			clearValues.push_back(VkClearValue{ .color = {0.0f, 0.0f, 0.0f, 1.0f} });
		}

		if (pass.passDepth.has_value()) {
			FrameGraphImageResource* pRes = mImageManager.getImage(pass.passDepth.value());
			imageResources[pass.passDepth.value()] = pRes;

			pRes->usageInfo.emplace_back(FrameGraphImageUsageInfo{
				.passIndex = static_cast<uint32_t>(passIndex),
				.stage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
				// Could be also read
				.access = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
				.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
				.isWrite = true,
				.aspect = VK_IMAGE_ASPECT_DEPTH_BIT,
			});

			(mBuiltPasses.end() - 1)->renderPass
				.addAttachment(pass.passDepth.value(), pRes->description.format, VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
				.subpassDepthRef(0, (mBuiltPasses.end() - 1)->renderPass.getAttachmentId(pass.passDepth.value()), VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, VK_IMAGE_ASPECT_DEPTH_BIT);

			clearValues.push_back(VkClearValue{ .depthStencil = {1.0f, 0} });
		}

		(mBuiltPasses.end() - 1)->renderPass.create();

		(mBuiltPasses.end() - 1)->passImageResources = imageResources;
		(mBuiltPasses.end() - 1)->renderPassClearValues = clearValues;
	}

	return *this;
}

sf::FrameGraph& sf::FrameGraph::executeGraph(vkw::CommandBuffer& commandBuffer) {
	uint32_t currentPassIndex = 0;

	for (BuiltPass& pass : mBuiltPasses) {
		pass.recordBarriers(commandBuffer, currentPassIndex);

		vkw::Framebuffer tempFramebuffer;
		tempFramebuffer
			.setCorePtr(&Window::getVk())
			.extent(pass.passFramebufferImages[0]->description.extent)
			.renderPass(pass.renderPass.getRenderPass());

		for (FrameGraphImageResource* pRes : pass.passFramebufferImages) {
			tempFramebuffer.framebufferAttachments.push_back(pRes->imageView.getImageView());
		}

		tempFramebuffer.framebufferInfo.attachmentCount = static_cast<uint32_t>(tempFramebuffer.framebufferAttachments.size());
		tempFramebuffer.framebufferInfo.pAttachments = &tempFramebuffer.framebufferAttachments[0];

		tempFramebuffer.create();

		VkRenderPassBeginInfo beginInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
		beginInfo.renderPass = pass.renderPass.getRenderPass();
		beginInfo.framebuffer = tempFramebuffer.getFramebuffer();
		beginInfo.renderArea = { .offset = {0, 0}, .extent = pass.passFramebufferImages[0]->description.extent };
		beginInfo.clearValueCount = static_cast<uint32_t>(pass.renderPassClearValues.size());
		beginInfo.pClearValues = &pass.renderPassClearValues[0];

		VkSubpassBeginInfo subpassBegin = { VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO };
		subpassBegin.contents = VK_SUBPASS_CONTENTS_INLINE;

		VkSubpassEndInfo subpassEnd = { VK_STRUCTURE_TYPE_SUBPASS_END_INFO };

		commandBuffer.cmdBeginRenderPass2(&beginInfo, &subpassBegin);
		pass.passExecuteCopy(commandBuffer, pass.passImageResources);
		commandBuffer.cmdEndRenderPass2(&subpassEnd);

		tempFramebuffer.destroy();

		currentPassIndex++;
	}

	return *this;
}

void sf::FrameGraph::destroy() {
	mImageManager.destroyAll();

	for (BuiltPass& pass : mBuiltPasses) {
		pass.renderPass.destroy();
		pass.passImageResources.clear();
		pass.passFramebufferImages.clear();
		pass.renderPassClearValues.clear();
	}

	mBuiltPasses.clear();
}