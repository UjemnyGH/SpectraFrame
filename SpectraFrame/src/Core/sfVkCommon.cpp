#include "sfVkCommon.h"
#include "sfWindow.h"

void sf::commandBufferOneTimeBegin(vkw::CommandBuffer& commandBuffer, vkw::CommandPool& commandPool) {
	commandBuffer
		.setCorePtr(&Window::getVk())
		.beginFlag(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT)
		.allocate(commandPool.getCommandPool())
		.begin();
}

void sf::commandBufferOneTimeEnd(vkw::CommandBuffer& commandBuffer, const VkQueue submitQueue) {
	commandBuffer.end();

	vkw::Submit submitCommandBuffer;
	submitCommandBuffer
		.addCommandBuffer(commandBuffer.getCommandBuffer())
		.submit(submitQueue, VK_NULL_HANDLE);

	vkQueueWaitIdle(submitQueue);

	commandBuffer.destroy();
}

sf::CopyBuffer& sf::CopyBuffer::setCommandBuffer(vkw::CommandBuffer& commandBuffer) {
	pCommandBuffer = &commandBuffer;

	return *this;
}

sf::CopyBuffer& sf::CopyBuffer::copyBufferToBuffer(vkw::Buffer& src, vkw::Buffer& dst, const VkDeviceSize srcDataSize) {
	VkBufferCopy bufferCopyRegion{};
	bufferCopyRegion.dstOffset = 0;
	bufferCopyRegion.srcOffset = 0;
	bufferCopyRegion.size = srcDataSize;

	pCommandBuffer->cmdCopyBuffer(src.getBuffer(), dst.getBuffer(), 1, &bufferCopyRegion);

	return *this;
}

sf::CopyBuffer& sf::CopyBuffer::copyBufferToImage(vkw::Buffer& src, vkw::Image& dst, const VkExtent3D imageExtent, const VkImageSubresourceLayers subresources) {
	VkBufferImageCopy bufferImageCopy{};
	bufferImageCopy.bufferOffset = 0;
	bufferImageCopy.bufferRowLength = 0;
	bufferImageCopy.bufferImageHeight = 0;
	bufferImageCopy.imageOffset = { 0, 0, 0 };
	bufferImageCopy.imageExtent = imageExtent;
	bufferImageCopy.imageSubresource = subresources;

	pCommandBuffer->cmdCopyBufferToImage(src.getBuffer(), dst.getImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferImageCopy);

	return *this;
}

sf::CopyBuffer& sf::CopyBuffer::copyImageToImage(vkw::Image& src, vkw::Image& dst, const VkExtent3D imageExtent, const VkImageLayout imageLayout, const VkImageSubresourceLayers subresources) {
	VkImageCopy imageCopy{};
	imageCopy.srcOffset = { 0, 0, 0 };
	imageCopy.dstOffset = { 0, 0, 0 };
	imageCopy.extent = imageExtent;
	imageCopy.srcSubresource = subresources;
	imageCopy.dstSubresource = subresources;
	
	pCommandBuffer->cmdCopyImage(src.getImage(), imageLayout, dst.getImage(), imageLayout, 1, &imageCopy);

	return *this;
}

sf::TransitionImage::TransitionImage() {
	imageMemoryBarrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
	pCommandBuffer = nullptr;
}

sf::TransitionImage& sf::TransitionImage::setCommandBuffer(vkw::CommandBuffer& commandBuffer) {
	pCommandBuffer = &commandBuffer;

	return *this;
}

sf::TransitionImage& sf::TransitionImage::layouts(const VkImageLayout oldL, const VkImageLayout newL) {
	imageMemoryBarrier.oldLayout = oldL;
	imageMemoryBarrier.newLayout = newL;

	return *this;
}

sf::TransitionImage& sf::TransitionImage::accessMask(const VkAccessFlags srcMask, const VkAccessFlags dstMask) {
	imageMemoryBarrier.srcAccessMask = srcMask;
	imageMemoryBarrier.dstAccessMask = dstMask;

	return *this;
}

sf::TransitionImage& sf::TransitionImage::subresources(const VkImageSubresourceRange subresources) {
	imageMemoryBarrier.subresourceRange = subresources;

	return *this;
}

sf::TransitionImage& sf::TransitionImage::transition(vkw::Image& image, const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask) {
	imageMemoryBarrier.image = image.getImage();

	pCommandBuffer->cmdPipelineBarrier(srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

	return *this;
}

sf::VertexBuffer::~VertexBuffer() {
	destroy();
}

vkw::Buffer& sf::VertexBuffer::getBuffer() {
	return mBuffer;
}

size_t sf::VertexBuffer::getBufferSize() {
	return mBufferSize;
}

sf::VertexBuffer& sf::VertexBuffer::update(vkw::CommandPool& commandPool, const VkQueue submitQueue, const std::vector<float>& data) {
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

	vkw::Buffer copyStageBuffer;
	copyStageBuffer
		.setCorePtr(commandPool.getCorePtr())
		.properties(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
		.usage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
		.create(mBufferSize)
		.map(&pCopyData, mBufferSize);

	memcpy(pCopyData, &data[0], mBufferSize);

	copyStageBuffer.unmap();

	vkw::CommandBuffer oneTimeCopy;
	
	commandBufferOneTimeBegin(oneTimeCopy, commandPool);

	CopyBuffer copyRegion;
	copyRegion
		.setCommandBuffer(oneTimeCopy)
		.copyBufferToBuffer(mBuffer, copyStageBuffer, mBufferSize);

	commandBufferOneTimeEnd(oneTimeCopy, submitQueue);

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

VkDescriptorType sf::UniformBuffer::getVkType() {
	return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
}

vkw::Buffer& sf::UniformBuffer::getBuffer() {
	return mBuffer;
}

size_t sf::UniformBuffer::getBufferSize() {
	return mBufferSize;
}

sf::UniformBuffer& sf::UniformBuffer::create(const size_t size) {
	mBufferSize = size;
	
	mBuffer
		.setCorePtr(&Window::getVk())
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

VkDescriptorType sf::Sampler::getVkType() {
	return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
}

vkw::Image& sf::Sampler::getImage() {
	return mImage;
}

vkw::ImageView& sf::Sampler::getImageView() {
	return mImageView;
}

vkw::Sampler& sf::Sampler::getSampler() {
	return mSampler;
}

VkExtent3D sf::Sampler::getImageExtent() {
	return mImageExtent;
}

uint32_t sf::Sampler::getMipmapLevels() {
	return mMipmapLevels;
}

sf::Sampler& sf::Sampler::create(vkw::CommandPool& commandPool, const VkQueue submitQueue, const uint8_t* const pixels, const size_t pixelsSize, const VkExtent3D extent, const VkFormat imageFormat, const uint32_t mipLevels) {
	mImageExtent = extent;
	mMipmapLevels = mipLevels;

	void* pPixelData;

	vkw::Buffer copyImageBuffer;
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

	vkw::CommandBuffer oneTimeCopy;

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

VkDescriptorType sf::ShaderStorageBuffer::getVkType() {
	return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
}

vkw::Buffer& sf::ShaderStorageBuffer::getBuffer() {
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
			.setCorePtr(&Window::getVk())
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

vkw::DescriptorPool& sf::Descriptor::getPool() {
	return mPool;
}

vkw::DescriptorSet& sf::Descriptor::getSet() {
	return mSet;
}

vkw::DescriptorSetLayout& sf::Descriptor::getLayout() {
	return mLayout;
}

const std::vector<vkw::WriteDescriptorSet>& sf::Descriptor::getWriteSets() {
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

sf::Descriptor& sf::Descriptor::addUniform(UniformBuffer& uniformBuffer, const uint32_t binding, const VkShaderStageFlags shaderStage) {
	mUniformBufferCount++;

	mWriteSets.push_back(vkw::WriteDescriptorSet());
	mWriteSets.end()->
		type(UniformBuffer::getVkType())
		.asUniformBuffer(uniformBuffer.getBuffer().getBuffer(), uniformBuffer.getBufferSize())
		.binding(binding)
		.setDescriptorSet(mSet.getSet());

	mLayout.addBinding(binding, 1, UniformBuffer::getVkType(), shaderStage);

	return *this;
}

sf::Descriptor& sf::Descriptor::addSampler(Sampler& sampler, const uint32_t binding, const VkShaderStageFlags shaderStage) {
	mCombinedImageSamplerCount++;

	mWriteSets.push_back(vkw::WriteDescriptorSet());
	mWriteSets.end()->
		type(Sampler::getVkType())
		.asSampler(sampler.getImageView().getImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, sampler.getSampler().getSampler())
		.binding(binding)
		.setDescriptorSet(mSet.getSet());

	mLayout.addBinding(binding, 1, Sampler::getVkType(), shaderStage);

	return *this;
}

sf::Descriptor& sf::Descriptor::addShaderStorage(ShaderStorageBuffer& shaderStorage, const uint32_t binding, const VkShaderStageFlags shaderStage) {
	mShaderStorageBufferCount++;

	mWriteSets.push_back(vkw::WriteDescriptorSet());
	mWriteSets.end()->
		type(ShaderStorageBuffer::getVkType())
		.asUniformBuffer(shaderStorage.getBuffer().getBuffer(), shaderStorage.getBufferSize())
		.binding(binding)
		.setDescriptorSet(mSet.getSet());

	mLayout.addBinding(binding, 1, ShaderStorageBuffer::getVkType(), shaderStage);

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
		.setCorePtr(&Window::getVk())
		.flags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
		.maxSets(1);

	if (mUniformBufferCount > 0) mPool.addSize(UniformBuffer::getVkType(), mUniformBufferCount);
	if (mCombinedImageSamplerCount > 0) mPool.addSize(Sampler::getVkType(), mCombinedImageSamplerCount);
	if (mShaderStorageBufferCount > 0) mPool.addSize(ShaderStorageBuffer::getVkType(), mShaderStorageBufferCount);

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

vkw::Swapchain& sf::SwapchainHandler::getSwapchain() {
	return mSwapchain;
}

std::vector<vkw::ImageView>& sf::SwapchainHandler::getImageViews() {
	return mSwapchainImageViews;
}

std::vector<vkw::Framebuffer>& sf::SwapchainHandler::getFramebuffers() {
	return mSwapchainFramebuffers;
}

vkw::Image& sf::SwapchainHandler::getDepthImage() {
	return mDepthImage;
}

vkw::ImageView& sf::SwapchainHandler::getDepthImageView() {
	return mDepthImageView;
}

uint32_t sf::SwapchainHandler::getNextImageIndex() {
	return mNextImageIndex;
}

bool sf::SwapchainHandler::shouldSkipFrame() {
	return mSkipFrame;
}

vkw::Semaphore& sf::SwapchainHandler::getImageAvailableSemaphore() {
	return mImageAvailable;
}

vkw::Semaphore& sf::SwapchainHandler::getQueueSubmitFinishedSemaphore() {
	return mQueueSubmitFinished;
}

vkw::Fence& sf::SwapchainHandler::getFlightFence() {
	return mFlight;
}

sf::SwapchainHandler& sf::SwapchainHandler::createSwapchain(vkw::CommandPool& commandPool, const VkQueue submitQueue, const VkRenderPass renderPass, const VkExtent2D extent) {
	mSwapchain
		.setCorePtr(&Window::getVk())
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

	vkw::CommandBuffer oneTimeBuffer;

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
		.setCorePtr(&Window::getVk())
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

sf::SwapchainHandler& sf::SwapchainHandler::aquireNextImage(vkw::CommandPool& commandPool, const VkQueue submitQueue, const VkRenderPass renderPass, const VkExtent2D extent) {
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

	VkResult result = vkAcquireNextImageKHR(mSwapchain.getCorePtr()->getDevice(), mSwapchain.getSwapchain(), ~0ULL, mImageAvailable.getSemaphore(), VK_NULL_HANDLE, &mNextImageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		Window::getVk().waitForDeviceIdle();
		Window::getVk().refreshSurfaceCapabilities();

		destroy();
		createSwapchain(commandPool, submitQueue, renderPass, extent);

		mSkipFrame = true;

		return *this;
	}

	mFlight.reset();

	return *this;
}

sf::SwapchainHandler& sf::SwapchainHandler::presentSwapchain(vkw::CommandPool& commandPool, const VkQueue submitQueue, const VkRenderPass renderPass, const VkExtent2D extent) {
	if (mSwapchain.getSwapchain() == VK_NULL_HANDLE) {
		destroy();
		createSwapchain(commandPool, submitQueue, renderPass, extent);
	}

	if (mImageAvailable.getSemaphore() == VK_NULL_HANDLE) {
		destroySyncObjects();
		createSyncObejcts();
	}
	
	vkw::Present present;
	VkResult result = present
		.addSwapchain(mSwapchain.getSwapchain())
		.addWait(mQueueSubmitFinished.getSemaphore())
		.present(submitQueue, &mNextImageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || mSwapchain.getExtent().width != extent.width || mSwapchain.getExtent().height != extent.height) {
		Window::getVk().waitForDeviceIdle();
		Window::getVk().refreshSurfaceCapabilities();

		destroy();
		createSwapchain(commandPool, submitQueue, renderPass, extent);
	}

	return *this;
}

void sf::SwapchainHandler::destroy() {
	for (vkw::Framebuffer& framebuffer : mSwapchainFramebuffers) {
		framebuffer.destroy();
	}

	for (vkw::ImageView& imageView : mSwapchainImageViews) {
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

vkw::ShaderModule& sf::Shader::getShaderModule() {
	return mShaderModule;
}

VkShaderStageFlags sf::Shader::getShaderStage() {
	return mShaderStage;
}

sf::Shader& sf::Shader::create(const std::vector<uint8_t>& shaderBinary, const VkShaderStageFlags shaderStage) {
	mShaderStage = shaderStage;

	mShaderModule
		.setCorePtr(&Window::getVk())
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

vkw::PipelineLayout& sf::Pipeline::getPipelineLayout() {
	return mPipelineLayout;
}

sf::Pipeline& sf::Pipeline::addShader(Shader& shader) {
	addShaderStage(shader.getShaderModule().getShaderModule(), (VkShaderStageFlagBits)shader.getShaderStage());

	return *this;
}

sf::Pipeline& sf::Pipeline::addDataBinding(const uint32_t dataSizePerVertex, const uint32_t binding) {
	addBinding(binding, dataSizePerVertex);

	return *this;
}

sf::Pipeline& sf::Pipeline::addDataAttribute(const uint32_t location, const VkFormat format, const uint32_t dataOffset, const uint32_t binding) {
	addAttribute(location, binding, format, dataOffset);

	return *this;
}

sf::Pipeline& sf::Pipeline::enableDepth(const VkCompareOp op, float minDepth, float maxDepth) {
	this->
		depthBounds(minDepth, maxDepth)
		.depthCompareOp(op)
		.depthStencilEnable(VK_TRUE, VK_TRUE, VK_FALSE, VK_FALSE);

	return *this;
}

sf::Pipeline& sf::Pipeline::createGraphicsPipeline(const VkRenderPass renderPass, Descriptor& descriptor) {	
	mPipelineLayout
		.setCorePtr(descriptor.getLayout().getCorePtr())
		.addLayouts(descriptor.getLayout().getSetLayout())
		.create();
	
	this->
		setCorePtr(mPipelineLayout.getCorePtr())
		.layout(mPipelineLayout.getLayout())
		.renderPass(renderPass)
		.viewport(VkExtent2D{ (uint32_t)Window::sWindowInstancePtr->getWidth(), (uint32_t)Window::sWindowInstancePtr->getHeight() })
		.scissor(VkExtent2D{ (uint32_t)Window::sWindowInstancePtr->getWidth(), (uint32_t)Window::sWindowInstancePtr->getHeight() })
		.createGraphics();

	return *this;
}

void sf::Pipeline::destroyAll() {
	destroy();

	mPipelineLayout.destroy();
}