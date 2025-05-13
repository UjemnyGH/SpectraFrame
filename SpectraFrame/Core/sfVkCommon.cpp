#include "sfVkCommon.h"
#include "sfWindow.h"
#include <vulkan/vulkan.hpp>

void sf::commandBufferOneTimeBegin(vk::CommandBuffer& commandBuffer, vk::CommandPool& commandPool) {
  vk::CommandBufferAllocateInfo allocInfo;
  allocInfo
    .setCommandBufferCount(1)
    .setCommandPool(commandPool);

  Vulkan::device().allocateCommandBuffers(allocInfo, &commandBuffer);

  vk::CommandBufferBeginInfo beginInfo;
  beginInfo
    .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

  commandBuffer.begin(beginInfo);
}

void sf::commandBufferOneTimeEnd(vk::CommandBuffer& commandBuffer, vk::CommandPool& commandPool, const vk::Queue submitQueue) {
	commandBuffer.end();

	vk::SubmitInfo submitCommandBuffer;
	submitCommandBuffer
		.setCommandBufferCount(1)
    .setPCommandBuffers(&commandBuffer);

  Vulkan::graphicsQueue().submit(1, &submitCommandBuffer, (vk::Fence)nullptr);

	vkQueueWaitIdle(submitQueue);

  Vulkan::device().freeCommandBuffers(commandPool, 1, &commandBuffer);
}

sf::CopyBuffer& sf::CopyBuffer::setCommandBuffer(vk::CommandBuffer& commandBuffer) {
	pCommandBuffer = &commandBuffer;

	return *this;
}

sf::CopyBuffer& sf::CopyBuffer::copyBufferToBuffer(vk::Buffer& src, vk::Buffer& dst, const vk::DeviceSize srcDataSize) {
	vk::BufferCopy bufferCopyRegion{};
	bufferCopyRegion.dstOffset = 0;
	bufferCopyRegion.srcOffset = 0;
	bufferCopyRegion.size = srcDataSize;

	pCommandBuffer->copyBuffer(src, dst, 1, &bufferCopyRegion);

	return *this;
}

sf::CopyBuffer& sf::CopyBuffer::copyBufferToImage(vk::Buffer& src, vk::Image& dst, const vk::Extent3D imageExtent, const vk::ImageSubresourceLayers subresources) {
	vk::BufferImageCopy bufferImageCopy{};
	bufferImageCopy.bufferOffset = 0;
	bufferImageCopy.bufferRowLength = 0;
	bufferImageCopy.bufferImageHeight = 0;
	bufferImageCopy.imageOffset = (vk::Offset3D) { 0, 0, 0 };
	bufferImageCopy.imageExtent = imageExtent;
	bufferImageCopy.imageSubresource = subresources;

  pCommandBuffer->copyBufferToImage(src, dst, vk::ImageLayout::eTransferDstOptimal, 1, &bufferImageCopy);

	return *this;
}

sf::CopyBuffer& sf::CopyBuffer::copyImageToImage(vk::Image& src, vk::Image& dst, const vk::Extent3D imageExtent, const vk::ImageLayout imageLayout, const vk::ImageSubresourceLayers subresources) {
	vk::ImageCopy imageCopy{};
	imageCopy.srcOffset = (vk::Offset3D) { 0, 0, 0 };
	imageCopy.dstOffset = (vk::Offset3D) { 0, 0, 0 };
	imageCopy.extent = imageExtent;
	imageCopy.srcSubresource = subresources;
	imageCopy.dstSubresource = subresources;
	
	pCommandBuffer->copyImage(src, imageLayout, dst, imageLayout, 1, &imageCopy);

	return *this;
}

sf::TransitionImage::TransitionImage() {
	imageMemoryBarrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
	pCommandBuffer = nullptr;
}

sf::TransitionImage& sf::TransitionImage::setCommandBuffer(vk::CommandBuffer& commandBuffer) {
	pCommandBuffer = &commandBuffer;

	return *this;
}

sf::TransitionImage& sf::TransitionImage::layouts(const vk::ImageLayout oldL, const vk::ImageLayout newL) {
	imageMemoryBarrier.oldLayout = oldL;
	imageMemoryBarrier.newLayout = newL;

	return *this;
}

sf::TransitionImage& sf::TransitionImage::accessMask(const vk::AccessFlags srcMask, const vk::AccessFlags dstMask) {
	imageMemoryBarrier.srcAccessMask = srcMask;
	imageMemoryBarrier.dstAccessMask = dstMask;

	return *this;
}

sf::TransitionImage& sf::TransitionImage::subresources(const vk::ImageSubresourceRange subresources) {
	imageMemoryBarrier.subresourceRange = subresources;

	return *this;
}

sf::TransitionImage& sf::TransitionImage::transition(vk::Image& image, const vk::PipelineStageFlags srcStageMask, const vk::PipelineStageFlags dstStageMask) {
	imageMemoryBarrier.image = image;
  
	pCommandBuffer->pipelineBarrier(srcStageMask, dstStageMask, (vk::DependencyFlagBits) 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

	return *this;
}

sf::VertexBuffer::~VertexBuffer() {
	destroy();
}

vk::Buffer& sf::VertexBuffer::getBuffer() {
	return mBuffer;
}

size_t sf::VertexBuffer::getBufferSize() {
	return mBufferSize;
}

template <typename T>
sf::VertexBuffer& sf::VertexBuffer::update(vk::CommandPool& commandPool, const vk::Queue submitQueue, const std::vector<T>& data) {
	if (mBufferSize != data.size() * sizeof(data[0]) || mBuffer.getBuffer() == VK_NULL_HANDLE || mBuffer.getMemory() == VK_NULL_HANDLE) {
		destroy();

		mBufferSize = data.size() * sizeof(data[0]);

		mBuffer
			.setCorePtr(commandPool.getCorePtr())
			.properties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
			.usage(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)
			.create(mBufferSize);
	}

	void* pCopyData;

	vk::Buffer copyStageBuffer;
	copyStageBuffer
		.setCorePtr(commandPool.getCorePtr())
		.properties(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
		.usage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
		.create(mBufferSize)
		.map(&pCopyData, mBufferSize);

	memcpy(pCopyData, &data[0], mBufferSize);

	copyStageBuffer.unmap();

	vk::CommandBuffer oneTimeCopy;
	
	commandBufferOneTimeBegin(oneTimeCopy, commandPool);

	CopyBuffer copyRegion;
	copyRegion
		.setCommandBuffer(oneTimeCopy)
		.copyBufferToBuffer(mBuffer, copyStageBuffer, mBufferSize);

	commandBufferOneTimeEnd(oneTimeCopy, commandPool, submitQueue);

	copyStageBuffer.destroy();

	return *this;
}

void sf::VertexBuffer::destroy() {
	mBuffer.destroy();

	mBufferSize = 0;
}

sf::UniformBuffer::~UniformBuffer() {
	destroy();
}

vk::DescriptorType sf::UniformBuffer::getVkType() {
	return vk::DescriptorType::eUniformBuffer;
}

vk::Buffer& sf::UniformBuffer::getBuffer() {
	return mBuffer;
}

size_t sf::UniformBuffer::getBufferSize() {
	return mBufferSize;
}

sf::UniformBuffer& sf::UniformBuffer::create(const size_t size) {
	mBufferSize = size;
	
	mBuffer
		.setCorePtr(&Window::getvk::())
		.properties(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
		.usage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
		.create(mBufferSize);

	return *this;
}

sf::UniformBuffer& sf::UniformBuffer::map(void** ppDataPlace) {
	mBuffer.map(ppDataPlace, mBufferSize);

	return *this;
}

sf::UniformBuffer& sf::UniformBuffer::unmap() {
	mBuffer.unmap();

	return *this;
}

void sf::UniformBuffer::destroy() {
	mBuffer.destroy();
}

sf::Sampler::~Sampler() {
	destroy();
}

vk::DescriptorType sf::Sampler::getvk::Type() {
	return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
}

vk::Image& sf::Sampler::getImage() {
	return mImage;
}

vk::ImageView& sf::Sampler::getImageView() {
	return mImageView;
}

vk::Sampler& sf::Sampler::getSampler() {
	return mSampler;
}

vk::Extent3D sf::Sampler::getImageExtent() {
	return mImageExtent;
}

uint32_t sf::Sampler::getMipmapLevels() {
	return mMipmapLevels;
}

sf::Sampler& sf::Sampler::create(vk::CommandPool& commandPool, const vk::Queue submitQueue, const uint8_t* const pixels, const size_t pixelsSize, const vk::Extent3D extent, const vk::Format imageFormat, const uint32_t mipLevels) {
	mImageExtent = extent;
	mMipmapLevels = mipLevels;

	void* pPixelData;

	vk::Buffer copyImageBuffer;
	copyImageBuffer
		.setCorePtr(commandPool.getCorePtr())
		.usage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
		.properties(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
		.create(pixelsSize)
		.map(&pPixelData, pixelsSize);

	memcpy(pPixelData, pixels, pixelsSize);

	copyImageBuffer.unmap();

	mImage
		.setCorePtr(commandPool.getCorePtr())
		.properties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
		.usage(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT)
		.extent(mImageExtent)
		.mipmapLevels(mMipmapLevels)
		.create();

	vk::CommandBuffer oneTimeCopy;

	commandBufferOneTimeBegin(oneTimeCopy, commandPool);

	TransitionImage transitionImage;
	transitionImage
		.setCommandBuffer(oneTimeCopy)
		.layouts(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		.accessMask(0, VK_ACCESS_TRANSFER_WRITE_BIT)
		// aspectMask, baseMip, levelCount, baseArray, layerCount
		.subresources({ VK_IMAGE_ASPECT_COLOR_BIT, 0, mMipmapLevels, 0, 1 })
		.transition(mImage, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);

	CopyBuffer copyImage;
	copyImage
		.setCommandBuffer(oneTimeCopy)
		// aspectMask, mipLevel, baseArray, layerCount
		.copyBufferToImage(copyImageBuffer, mImage, mImageExtent, { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 });
		

	TransitionImage transitionImageShaderReady;
	transitionImageShaderReady
		.setCommandBuffer(oneTimeCopy)
		.layouts(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		.accessMask(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT)
		.subresources({ VK_IMAGE_ASPECT_COLOR_BIT, 0, mMipmapLevels, 0, 1 })
		.transition(mImage, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

	commandBufferOneTimeEnd(oneTimeCopy, submitQueue);

	copyImageBuffer.destroy();

	mImageView
		.setCorePtr(commandPool.getCorePtr())
		.format(imageFormat)
		.subresource(VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1, mMipmapLevels)
		.create(mImage.getImage());

	mSampler
		.setCorePtr(commandPool.getCorePtr())
		.maxAnisotropy(-1);

	if (mipLevels > 1)
		mSampler.mipmap(VK_SAMPLER_MIPMAP_MODE_LINEAR, 0.0f, 0.0f, static_cast<float>(mMipmapLevels));

	mSampler.create();

	return *this;
}

void sf::Sampler::destroy() {
	mSampler.destroy();

	mImageView.destroy();
	
	mImage.destroy();
}

sf::ShaderStorageBuffer::~ShaderStorageBuffer() {
	destroy();
}

vk::DescriptorType sf::ShaderStorageBuffer::getvk::Type() {
	return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
}

vk::Buffer& sf::ShaderStorageBuffer::getBuffer() {
	return mBuffer;
}

uint32_t sf::ShaderStorageBuffer::getBufferSize() {
	return mBufferSize;
}

sf::ShaderStorageBuffer& sf::ShaderStorageBuffer::update(void* pData, const size_t size) {
	if (mBufferSize != size || mBuffer.getBuffer() == VK_NULL_HANDLE) {
		destroy();

		mBufferSize = size;

		mBuffer
			.setCorePtr(&Window::getvk::())
			.properties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
			.usage(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
			.create(mBufferSize);
	}

	void* tempMemCopy;
	map(&tempMemCopy);
	memcpy(tempMemCopy, pData, mBufferSize);
	unmap();

	return *this;
}

sf::ShaderStorageBuffer& sf::ShaderStorageBuffer::map(void** ppDataPlace) {
	mBuffer.map(ppDataPlace, mBufferSize);

	return *this;
}

sf::ShaderStorageBuffer& sf::ShaderStorageBuffer::unmap() {
	mBuffer.unmap();

	return *this;
}

void sf::ShaderStorageBuffer::destroy() {
	mBufferSize = 0;

	mBuffer.destroy();
}

sf::Descriptor::~Descriptor() {
	destroy();
}

vk::DescriptorPool& sf::Descriptor::getPool() {
	return mPool;
}

vk::DescriptorSet& sf::Descriptor::getSet() {
	return mSet;
}

vk::DescriptorSetLayout& sf::Descriptor::getLayout() {
	return mLayout;
}

const std::vector<vk::WriteDescriptorSet>& sf::Descriptor::getWriteSets() {
	return mWriteSets;
}

uint32_t sf::Descriptor::getUniformBufferAmount() {
	return mUniformBufferCount;
}

uint32_t sf::Descriptor::getCombinedImageSamplerAmount() {
	return mCombinedImageSamplerCount;
}

uint32_t sf::Descriptor::getShaderStorageBufferAmount() {
	return mShaderStorageBufferCount;
}

sf::Descriptor& sf::Descriptor::addUniform(UniformBuffer& uniformBuffer, const uint32_t binding, const vk::ShaderStageFlags shaderStage) {
	mUniformBufferCount++;

	mWriteSets.push_back(vk::WriteDescriptorSet());
	mWriteSets.end()->
		type(UniformBuffer::getvk::Type())
		.asUniformBuffer(uniformBuffer.getBuffer().getBuffer(), uniformBuffer.getBufferSize())
		.binding(binding)
		.setDescriptorSet(mSet.getSet());

	mLayout.addBinding(binding, 1, UniformBuffer::getvk::Type(), shaderStage);

	return *this;
}

sf::Descriptor& sf::Descriptor::addSampler(Sampler& sampler, const uint32_t binding, const vk::ShaderStageFlags shaderStage) {
	mCombinedImageSamplerCount++;

	mWriteSets.push_back(vk::WriteDescriptorSet());
	mWriteSets.end()->
		type(Sampler::getvk::Type())
		.asSampler(sampler.getImageView().getImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, sampler.getSampler().getSampler())
		.binding(binding)
		.setDescriptorSet(mSet.getSet());

	mLayout.addBinding(binding, 1, Sampler::getvk::Type(), shaderStage);

	return *this;
}

sf::Descriptor& sf::Descriptor::addShaderStorage(ShaderStorageBuffer& shaderStorage, const uint32_t binding, const vk::ShaderStageFlags shaderStage) {
	mShaderStorageBufferCount++;

	mWriteSets.push_back(vk::WriteDescriptorSet());
	mWriteSets.end()->
		type(ShaderStorageBuffer::getvk::Type())
		.asUniformBuffer(shaderStorage.getBuffer().getBuffer(), shaderStorage.getBufferSize())
		.binding(binding)
		.setDescriptorSet(mSet.getSet());

	mLayout.addBinding(binding, 1, ShaderStorageBuffer::getvk::Type(), shaderStage);

	return *this;
}

sf::Descriptor& sf::Descriptor::clear() {
	mCombinedImageSamplerCount = 0;
	mUniformBufferCount = 0;
	mShaderStorageBufferCount = 0;

	mWriteSets.clear();

	return *this;
}

sf::Descriptor& sf::Descriptor::create() {
	mPool
		.setCorePtr(&Window::getvk::())
		.flags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
		.maxSets(1);

	if (mUniformBufferCount > 0) mPool.addSize(UniformBuffer::getvk::Type(), mUniformBufferCount);
	if (mCombinedImageSamplerCount > 0) mPool.addSize(Sampler::getvk::Type(), mCombinedImageSamplerCount);
	if (mShaderStorageBufferCount > 0) mPool.addSize(ShaderStorageBuffer::getvk::Type(), mShaderStorageBufferCount);

	mLayout
		.setCorePtr(mPool.getCorePtr())
		.create();

	mPool.create();

	mSet
		.setCorePtr(mPool.getCorePtr())
		.setLayouts(&mLayout.getSetLayout())
		.allocate(mPool.getPool())
		.update(mWriteSets);

	return *this;
}

void sf::Descriptor::destroy() {
	clear();

	mSet.destroy();

	mPool.destroy();

	mLayout.destroy();
	mLayout.clearBindings();
}

sf::SwapchainHandler::~SwapchainHandler() {
	destroy();

	destroySyncObjects();
}

vk::Swapchain& sf::SwapchainHandler::getSwapchain() {
	return mSwapchain;
}

std::vector<vk::ImageView>& sf::SwapchainHandler::getImageViews() {
	return mSwapchainImageViews;
}

std::vector<vk::Framebuffer>& sf::SwapchainHandler::getFramebuffers() {
	return mSwapchainFramebuffers;
}

vk::Image& sf::SwapchainHandler::getDepthImage() {
	return mDepthImage;
}

vk::ImageView& sf::SwapchainHandler::getDepthImageView() {
	return mDepthImageView;
}

uint32_t sf::SwapchainHandler::getNextImageIndex() {
	return mNextImageIndex;
}

bool sf::SwapchainHandler::shouldSkipFrame() {
	return mSkipFrame;
}

vk::Semaphore& sf::SwapchainHandler::getImageAvailableSemaphore() {
	return mImageAvailable;
}

vk::Semaphore& sf::SwapchainHandler::getQueueSubmitFinishedSemaphore() {
	return mQueueSubmitFinished;
}

vk::Fence& sf::SwapchainHandler::getFlightFence() {
	return mFlight;
}

sf::SwapchainHandler& sf::SwapchainHandler::createSwapchain(vk::CommandPool& commandPool, const vk::Queue submitQueue, const vk::RenderPass renderPass, const vk::Extent2D extent) {
	mSwapchain
		.setCorePtr(&Window::getvk::())
		.create(extent);

	mSwapchainImageViews.resize(mSwapchain.getImagesAmount());

	for (uint32_t i = 0; i < mSwapchainImageViews.size(); i++) {
		mSwapchainImageViews[i]
			.setCorePtr(mSwapchain.getCorePtr())
			.create(mSwapchain.swapchainImages[i]);
	}

	mDepthImage
		.setCorePtr(mSwapchain.getCorePtr())
		.extent({ mSwapchain.getExtent().width, mSwapchain.getExtent().height, 1 })
		.properties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
		.usage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
		.format(/*TODO: Choose from gpu best format*/VK_FORMAT_D32_SFLOAT_S8_UINT)
		.create();

	mDepthImageView
		.setCorePtr(mSwapchain.getCorePtr())
		.format(/*TODO: Choose from gpu best format*/VK_FORMAT_D32_SFLOAT_S8_UINT)
		.subresource(VK_IMAGE_ASPECT_DEPTH_BIT)
		.create(mDepthImage.getImage());

	vk::CommandBuffer oneTimeBuffer;

	commandBufferOneTimeBegin(oneTimeBuffer, commandPool);

	TransitionImage transitionDepth;
	transitionDepth
		.setCommandBuffer(oneTimeBuffer)
		.layouts(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		.accessMask(0, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT)
		.subresources({ VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, 0, 1, 0, 1 })
		.transition(mDepthImage, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT);

	commandBufferOneTimeEnd(oneTimeBuffer, submitQueue);

	mSwapchainFramebuffers.resize(mSwapchainImageViews.size());

	for (uint32_t i = 0; i < mSwapchainFramebuffers.size(); i++) {
		mSwapchainFramebuffers[i]
			.setCorePtr(mSwapchain.getCorePtr())
			.addAttachment(mSwapchainImageViews[i].getImageView())
			.addAttachment(mDepthImageView.getImageView())
			.extent(mSwapchain.getExtent())
			.renderPass(renderPass)
			.create();
	}

	return *this;
}

sf::SwapchainHandler& sf::SwapchainHandler::createSyncObejcts() {
	mImageAvailable
		.setCorePtr(&Window::getvk::())
		.create();

	mQueueSubmitFinished
		.setCorePtr(mImageAvailable.getCorePtr())
		.create();

	mFlight
		.setCorePtr(mImageAvailable.getCorePtr())
		.signaled()
		.create();

	return *this;
}

sf::SwapchainHandler& sf::SwapchainHandler::aquireNextImage(vk::CommandPool& commandPool, const vk::Queue submitQueue, const vk::RenderPass renderPass, const vk::Extent2D extent) {
	if (mSwapchain.getSwapchain() == VK_NULL_HANDLE) {
		destroy();
		createSwapchain(commandPool, submitQueue, renderPass, extent);
	}

	if (mImageAvailable.getSemaphore() == VK_NULL_HANDLE) {
		destroySyncObjects();
		createSyncObejcts();
	}

	mSkipFrame = false;

	mFlight.wait();

	vk::Result result = vkAcquireNextImageKHR(mSwapchain.getCorePtr()->getDevice(), mSwapchain.getSwapchain(), ~0ULL, mImageAvailable.getSemaphore(), VK_NULL_HANDLE, &mNextImageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		Window::getvk::().waitForDeviceIdle();
		Window::getvk::().refreshSurfaceCapabilities();

		destroy();
		createSwapchain(commandPool, submitQueue, renderPass, extent);

		mSkipFrame = true;

		return *this;
	}

	mFlight.reset();

	return *this;
}

sf::SwapchainHandler& sf::SwapchainHandler::presentSwapchain(vk::CommandPool& commandPool, const vk::Queue submitQueue, const vk::RenderPass renderPass, const vk::Extent2D extent) {
	if (mSwapchain.getSwapchain() == VK_NULL_HANDLE) {
		destroy();
		createSwapchain(commandPool, submitQueue, renderPass, extent);
	}

	if (mImageAvailable.getSemaphore() == VK_NULL_HANDLE) {
		destroySyncObjects();
		createSyncObejcts();
	}
	
	vk::Present present;
	vk::Result result = present
		.addSwapchain(mSwapchain.getSwapchain())
		.addWait(mQueueSubmitFinished.getSemaphore())
		.present(submitQueue, &mNextImageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || mSwapchain.getExtent().width != extent.width || mSwapchain.getExtent().height != extent.height) {
		Window::getvk::().waitForDeviceIdle();
		Window::getvk::().refreshSurfaceCapabilities();

		destroy();
		createSwapchain(commandPool, submitQueue, renderPass, extent);
	}

	return *this;
}

void sf::SwapchainHandler::destroy() {
	for (vk::Framebuffer& framebuffer : mSwapchainFramebuffers) {
		framebuffer.destroy();
	}

	for (vk::ImageView& imageView : mSwapchainImageViews) {
		imageView.destroy();
	}

	mDepthImage.destroy();

	mDepthImageView.destroy();

	mSwapchain.destroy();
}

void sf::SwapchainHandler::destroySyncObjects() {
	mImageAvailable.destroy();

	mQueueSubmitFinished.destroy();

	mFlight.destroy();
}

sf::Shader::~Shader() {
	destroy();
}

vk::ShaderModule& sf::Shader::getShaderModule() {
	return mShaderModule;
}

vk::ShaderStageFlags sf::Shader::getShaderStage() {
	return mShaderStage;
}

sf::Shader& sf::Shader::create(const std::vector<uint8_t>& shaderBinary, const vk::ShaderStageFlags shaderStage) {
	mShaderStage = shaderStage;

	mShaderModule
		.setCorePtr(&Window::getvk::())
		.create(&shaderBinary[0], shaderBinary.size());

	return *this;
}

void sf::Shader::destroy() {
	mShaderModule.destroy();

	mShaderStage = 0;
}

sf::Pipeline::~Pipeline() {
	destroyAll();
}

vk::PipelineLayout& sf::Pipeline::getPipelineLayout() {
	return mPipelineLayout;
}

sf::Pipeline& sf::Pipeline::addShader(Shader& shader) {
	addShaderStage(shader.getShaderModule().getShaderModule(), (vk::ShaderStageFlagBits)shader.getShaderStage());

	return *this;
}

sf::Pipeline& sf::Pipeline::addDataBinding(const uint32_t dataSizePerVertex, const uint32_t binding) {
	addBinding(binding, dataSizePerVertex);

	return *this;
}

sf::Pipeline& sf::Pipeline::addDataAttribute(const uint32_t location, const vk::Format format, const uint32_t dataOffset, const uint32_t binding) {
	addAttribute(location, binding, format, dataOffset);

	return *this;
}

sf::Pipeline& sf::Pipeline::enableDepth(const vk::CompareOp op, float minDepth, float maxDepth) {
	this->
		depthBounds(minDepth, maxDepth)
		.depthCompareOp(op)
		.depthStencilEnable(VK_TRUE, VK_TRUE, VK_FALSE, VK_FALSE);

	return *this;
}

sf::Pipeline& sf::Pipeline::createGraphicsPipeline(const vk::RenderPass renderPass, Descriptor& descriptor) {	
	mPipelineLayout
		.setCorePtr(descriptor.getLayout().getCorePtr())
		.addLayouts(descriptor.getLayout().getSetLayout())
		.create();
	
	this->
		setCorePtr(mPipelineLayout.getCorePtr())
		.layout(mPipelineLayout.getLayout())
		.renderPass(renderPass)
		.viewport(vk::Extent2D{ (uint32_t)Window::sWindowInstancePtr->getWidth(), (uint32_t)Window::sWindowInstancePtr->getHeight() })
		.scissor(vk::Extent2D{ (uint32_t)Window::sWindowInstancePtr->getWidth(), (uint32_t)Window::sWindowInstancePtr->getHeight() })
		.createGraphics();

	return *this;
}

void sf::Pipeline::destroyAll() {
	destroy();

	mPipelineLayout.destroy();
}

sf::DepthImage::~DepthImage() {
	destroy();
}

vk::Image& sf::DepthImage::getImage() {
	return mDepthImage;
}

vk::ImageView& sf::DepthImage::getView() {
	return mDepthImageView;
}

sf::DepthImage& sf::DepthImage::create(const vk::Extent3D extent, vk::CommandPool& pool) {
	mDepthImage
		.setCorePtr(&sf::Window::getvk::())
		.extent(extent)
		.properties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
		.usage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
		.format(sf::Window::getvk::().getDepthFormat())
		.create();

	mDepthImageView
		.setCorePtr(&sf::Window::getvk::())
		.format(sf::Window::getvk::().getDepthFormat())
		.subresource(VK_IMAGE_ASPECT_DEPTH_BIT)
		.create(mDepthImage.getImage());

	vk::CommandBuffer oneTimeBuffer;

	commandBufferOneTimeBegin(oneTimeBuffer, pool);

	TransitionImage depthBarrier;
	depthBarrier
		.setCommandBuffer(oneTimeBuffer)
		.layouts(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		.accessMask(0, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT)
		.subresources(
			vk::ImageSubresourceRange{
				.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}
		)
		.transition(mDepthImage, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT);

	commandBufferOneTimeEnd(oneTimeBuffer, sf::Window::getvk::().getGraphicsQueue());

	return *this;
}

void sf::DepthImage::destroy() {
	mDepthImageView.destroy();
	mDepthImage.destroy();
}
