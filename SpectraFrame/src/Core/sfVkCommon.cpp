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

sf::UniformBuffer& sf::UniformBuffer::map(void** dataPlace) {
	mBuffer.map(dataPlace, mBufferSize);

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

sf::Sampler& sf::Sampler::create(const uint8_t* const pixels, const size_t pixelsSize, const VkExtent3D extent, const VkFormat imageFormat, const uint32_t mipLevels) {
	mImageExtent = extent;
	mMipmapLevels = mipLevels;

	void* pPixelData;

	vkw::Buffer copyImageBuffer;
	copyImageBuffer
		.setCorePtr(&Window::getVk())
		.usage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
		.properties(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
		.create(pixelsSize)
		.map(&pPixelData, pixelsSize);

	memcpy(pPixelData, pixels, pixelsSize);


	// TODO: Finish


	return *this;
}

void sf::Sampler::destroy() {
	mSampler.destroy();

	mImageView.destroy();
	
	mImage.destroy();
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

const std::vector<vkw::WriteDescriptorSet>& sf::Descriptor::getWriteSets() {
	return mWriteSets;
}

sf::Descriptor& sf::Descriptor::addUniform(UniformBuffer& uniformBuffer, const uint32_t binding, const VkShaderStageFlags shaderStage, const uint32_t count) {
	mUniformBufferCount++;

	mWriteSets.push_back(vkw::WriteDescriptorSet());
	mWriteSets.end()->
		type(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
		.asUniformBuffer(uniformBuffer.getBuffer().getBuffer(), uniformBuffer.getBufferSize())
		.binding(binding)
		.setDescriptorSet(mSet.getSet());

	return *this;
}

//Descriptor& addSampler();

sf::Descriptor& sf::Descriptor::clear() {
	mWriteSets.clear();

	return *this;
}

sf::Descriptor& sf::Descriptor::create(vkw::DescriptorSetLayout& setLayout) {
	mPool
		.setCorePtr(&Window::getVk())
		.flags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
		.maxSets(1);

	if (mUniformBufferCount > 0) mPool.addSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, mUniformBufferCount);
	if (mCombinedImageSamplerCount > 0) mPool.addSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, mCombinedImageSamplerCount);

	mPool
		.create();

	mSet
		.setCorePtr(&Window::getVk())
		.setLayouts(&setLayout.getSetLayout())
		.allocate(mPool.getPool())
		.update(mWriteSets);

	return *this;
}

void sf::Descriptor::destroy() {
	clear();

	mSet.destroy();

	mPool.destroy();
}