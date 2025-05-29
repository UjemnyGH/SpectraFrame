#include "sfVkCommon.h"
#include "sfWindow.h"
#include "sfLogger.h"
#include <vulkan/vulkan.hpp>

void sf::commandBufferOneTimeBegin(vk::CommandBuffer& commandBuffer, vk::CommandPool& commandPool) {
  vk::CommandBufferAllocateInfo allocInfo;
  allocInfo
    .setCommandBufferCount(1)
    .setCommandPool(commandPool);

  if(Vulkan::device().allocateCommandBuffers(&allocInfo, &commandBuffer) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot allocate command buffer");

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

  if(Vulkan::graphicsQueue().submit(1, &submitCommandBuffer, (vk::Fence)nullptr) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot submit one time command buffer");

  vkQueueWaitIdle(submitQueue);

  Vulkan::device().freeCommandBuffers(commandPool, 1, &commandBuffer);
}

uint32_t sf::findImageMemoryType(vk::DeviceSize& rAllocationSize, vk::Image image, const vk::MemoryPropertyFlags properties) {
  vk::PhysicalDeviceMemoryProperties memProperties;
  Vulkan::gpu().getMemoryProperties(&memProperties);

  vk::MemoryRequirements memRequirements;
  Vulkan::device().getImageMemoryRequirements(image, &memRequirements);

  rAllocationSize = memRequirements.size;

  for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if(memRequirements.memoryTypeBits & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
      return i;
  }

  return ~0U;
}

uint32_t sf::findBufferMemoryType(vk::DeviceSize& rAllocationSize, vk::Buffer buffer, const vk::MemoryPropertyFlags properties) {
  vk::PhysicalDeviceMemoryProperties memProperties;
  Vulkan::gpu().getMemoryProperties(&memProperties);

  vk::MemoryRequirements memRequirements;
  Vulkan::device().getBufferMemoryRequirements(buffer, &memRequirements);

  rAllocationSize = memRequirements.size;

  for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if(memRequirements.memoryTypeBits & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
      return i;
  }

  return ~0U;
}

void sf::createVkFence(vk::Fence& rFence, bool signaled) {
  vk::FenceCreateInfo fenceInfo;
  fenceInfo.setFlags(signaled ? vk::FenceCreateFlagBits::eSignaled : vk::FenceCreateFlags(0));

  if(Vulkan::device().createFence(&fenceInfo, nullptr, &rFence) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create fence");
}

void sf::createVkSemaphore(vk::Semaphore& rSemaphore) {
  vk::SemaphoreCreateInfo semaphoreInfo;

  if(Vulkan::device().createSemaphore(&semaphoreInfo, nullptr, &rSemaphore) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create semaphore");
}

void sf::createVkCommandPool(vk::CommandPool& rCommandPool, const uint32_t queueIndex, const bool resetBuffers, const bool transient) {
  vk::CommandPoolCreateInfo cmdPoolInfo;
  cmdPoolInfo
    .setFlags((resetBuffers ? vk::CommandPoolCreateFlagBits::eResetCommandBuffer : vk::CommandPoolCreateFlags(0)) | (transient ? vk::CommandPoolCreateFlagBits::eTransient : vk::CommandPoolCreateFlags(0)))
    .setQueueFamilyIndex(queueIndex);

  if(Vulkan::device().createCommandPool(&cmdPoolInfo, nullptr, &rCommandPool) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create command pool");
}

void sf::allocateCommandBuffer(vk::CommandBuffer& rCommandBuffer, vk::CommandPool commandPool, const bool primary) {
  vk::CommandBufferAllocateInfo allocInfo;
  allocInfo
    .setCommandBufferCount(1)
    .setCommandPool(commandPool)
    .setLevel(primary ? vk::CommandBufferLevel::ePrimary : vk::CommandBufferLevel::eSecondary);

  if(Vulkan::device().allocateCommandBuffers(&allocInfo, &rCommandBuffer) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot allocate command buffer");
}

void sf::createVkRenderPassImage(vk::Image& rImage, vk::DeviceMemory& rMemory, const vk::Extent3D imageExtent, const vk::Format imageFormat) {
  vk::ImageCreateInfo imageInfo;
  imageInfo
    .setImageType(vk::ImageType::e2D)
    .setPQueueFamilyIndices(nullptr)
    .setQueueFamilyIndexCount(0)
    .setSharingMode(vk::SharingMode::eExclusive)
    .setExtent(imageExtent)
    .setArrayLayers(1)
    .setInitialLayout(vk::ImageLayout::eUndefined)
    .setSamples(vk::SampleCountFlagBits::e1)
    .setFormat(imageFormat)
    .setMipLevels(1)
    .setTiling(vk::ImageTiling::eOptimal);

  if(Vulkan::device().createImage(&imageInfo, nullptr, &rImage) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create render pass image");

  vk::MemoryAllocateInfo memAllocInfo;
  memAllocInfo.setMemoryTypeIndex(findImageMemoryType(memAllocInfo.allocationSize, rImage, vk::MemoryPropertyFlagBits::eDeviceLocal));

  if(Vulkan::device().allocateMemory(&memAllocInfo, nullptr, &rMemory) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot allocate memory for render pass image");

  Vulkan::device().bindImageMemory(rImage, rMemory, 0);
}

void sf::createVkImageView(vk::ImageView& rImageView, vk::Image image, const vk::Format format, const vk::ImageViewType viewType, const bool depthImageView) {
  vk::ImageViewCreateInfo imageViewInfo;
  imageViewInfo
    .setImage(image)
    .setComponents(vk::ComponentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA))
    .setViewType(vk::ImageViewType::e2D)
    .setFormat(format)
    .setViewType(viewType)
    .setSubresourceRange(vk::ImageSubresourceRange(depthImageView ? vk::ImageAspectFlagBits::eDepth : vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

  if(Vulkan::device().createImageView(&imageViewInfo, nullptr, &rImageView) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create image view");
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
  if (mBufferSize != data.size() * sizeof(data[0]) || mBuffer == nullptr || mBufferMemory == nullptr) {
    destroy();

    mBufferSize = data.size() * sizeof(data[0]);

    vk::BufferCreateInfo bufferInfo;
    bufferInfo
      .setSize(mBufferSize)
      .setSharingMode(vk::SharingMode::eExclusive)
      .setUsage(vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst)
      .setQueueFamilyIndexCount(0);

    if(Vulkan::device().createBuffer(&bufferInfo, nullptr, &mBuffer) != vk::Result::eSuccess)
      SF_CLOG("ERR: Cannot create vertex buffer buffer");

    vk::MemoryAllocateInfo memAllocInfo;
    memAllocInfo
      .setMemoryTypeIndex(findBufferMemoryType(memAllocInfo.allocationSize, mBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal));

    if(Vulkan::device().allocateMemory(&memAllocInfo, nullptr, &mBufferMemory) != vk::Result::eSuccess)
      SF_CLOG("ERR: Cannot allocate vertex buffer memory");

    Vulkan::device().bindBufferMemory(mBuffer, mBufferMemory, 0);
  }

  void* pCopyData;

  vk::Buffer stageBuffer;
  vk::DeviceMemory stageMemory;

  vk::BufferCreateInfo stageBufferInfo;
  stageBufferInfo
    .setSize(mBufferSize)
    .setUsage(vk::BufferUsageFlagBits::eTransferSrc)
    .setSharingMode(vk::SharingMode::eExclusive)
    .setQueueFamilyIndexCount(0);

  if(Vulkan::device().createBuffer(&stageBufferInfo, nullptr, &stageBuffer) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create vertex stage buffer buffer");

  vk::MemoryAllocateInfo stageMemAllocInfo;
  stageMemAllocInfo
    .setMemoryTypeIndex(findBufferMemoryType(stageMemAllocInfo.allocationSize, stageBuffer, vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible));

  if(Vulkan::device().allocateMemory(&stageMemAllocInfo, nullptr, &stageMemory) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot allocate vertex stage buffer memory");

  Vulkan::device().bindBufferMemory(stageBuffer, stageMemory, 0);

  Vulkan::device().mapMemory(stageMemory, 0, mBufferSize, vk::MemoryMapFlags(0), pCopyData);

  memcpy(pCopyData, &data[0], mBufferSize);

  Vulkan::device().unmapMemory(stageMemory);

  vk::CommandBuffer oneTimeCopy;
  
  commandBufferOneTimeBegin(oneTimeCopy, commandPool);

  CopyBuffer copyRegion;
  copyRegion
    .setCommandBuffer(oneTimeCopy)
    .copyBufferToBuffer(mBuffer, stageBuffer, mBufferSize);

  commandBufferOneTimeEnd(oneTimeCopy, commandPool, submitQueue);

  Vulkan::device().freeMemory(stageMemory, nullptr);
  Vulkan::device().destroyBuffer(stageBuffer, nullptr);

  return *this;
}

void sf::VertexBuffer::destroy() {
  Vulkan::device().freeMemory(mBufferMemory, nullptr);
  Vulkan::device().destroyBuffer(mBuffer, nullptr);

  mBufferSize = 0;
}

sf::UniformBuffer::~UniformBuffer() {
  destroy();
}

vk::Buffer& sf::UniformBuffer::getBuffer() {
  return mBuffer;
}

size_t sf::UniformBuffer::getBufferSize() {
  return mBufferSize;
}

sf::UniformBuffer& sf::UniformBuffer::create(const size_t size) {
  mBufferSize = size;
  
  vk::BufferCreateInfo bufferInfo;
  bufferInfo
    .setSharingMode(vk::SharingMode::eExclusive)
    .setSize(mBufferSize)
    .setUsage(vk::BufferUsageFlagBits::eUniformBuffer);

  if(Vulkan::device().createBuffer(&bufferInfo, nullptr, &mBuffer) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create uniform buffer buffer");

  vk::MemoryAllocateInfo memAllocInfo;
  memAllocInfo
    .setMemoryTypeIndex(findBufferMemoryType(memAllocInfo.allocationSize, mBuffer, vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible));

  if(Vulkan::device().allocateMemory(&memAllocInfo, nullptr, &mBufferMemory) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot allocate uniform buffer memory");

  Vulkan::device().bindBufferMemory(mBuffer, mBufferMemory, 0);

  return *this;
}

sf::UniformBuffer& sf::UniformBuffer::map(void** ppDataPlace) {
  Vulkan::device().mapMemory(mBufferMemory, 0, mBufferSize, vk::MemoryMapFlags(0), ppDataPlace);

  return *this;
}

sf::UniformBuffer& sf::UniformBuffer::unmap() {
  Vulkan::device().unmapMemory(mBufferMemory);

  return *this;
}

void sf::UniformBuffer::destroy() {
  Vulkan::device().freeMemory(mBufferMemory, nullptr);
  Vulkan::device().destroyBuffer(mBuffer, nullptr);
}

sf::Sampler::~Sampler() {
  destroy();
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

  vk::Buffer stageBuffer;
  vk::DeviceMemory stageMemory;

  vk::BufferCreateInfo stageBufferInfo;
  stageBufferInfo
    .setSharingMode(vk::SharingMode::eExclusive)
    .setUsage(vk::BufferUsageFlagBits::eTransferSrc)
    .setQueueFamilyIndexCount(0)
    .setSize(pixelsSize);

  if(Vulkan::device().createBuffer(&stageBufferInfo, nullptr, &stageBuffer) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create image stage copy buffer");

  vk::MemoryAllocateInfo memAllocInfo;
  memAllocInfo
    .setMemoryTypeIndex(findBufferMemoryType(memAllocInfo.allocationSize, stageBuffer, vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible));

  if(Vulkan::device().allocateMemory(&memAllocInfo, nullptr, &stageMemory) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot allocate memory for image stage copy buffer");

  Vulkan::device().bindBufferMemory(stageBuffer, stageMemory, 0);

  Vulkan::device().mapMemory(stageMemory, 0, pixelsSize, vk::MemoryMapFlags(0), &pPixelData);

  memcpy(pPixelData, pixels, pixelsSize);

  Vulkan::device().unmapMemory(stageMemory);

  vk::ImageCreateInfo imageInfo;
  imageInfo
    .setImageType(vk::ImageType::e2D)
    .setFormat(vk::Format::eR8G8B8A8Srgb)
    .setUsage(vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled)
    .setInitialLayout(vk::ImageLayout::eUndefined)
    .setTiling(vk::ImageTiling::eOptimal)
    .setExtent(mImageExtent)
    .setSharingMode(vk::SharingMode::eExclusive)
    .setMipLevels(mMipmapLevels)
    .setQueueFamilyIndexCount(0);

  if(Vulkan::device().createImage(&imageInfo, nullptr, &mImage) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create sampler image");

  memAllocInfo
    .setMemoryTypeIndex(findImageMemoryType(memAllocInfo.allocationSize, mImage, vk::MemoryPropertyFlagBits::eDeviceLocal));

  if(Vulkan::device().allocateMemory(&memAllocInfo, nullptr, &mImageMemory) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot allocate sampler image memory");

  Vulkan::device().bindImageMemory(mImage, mImageMemory, 0);

  vk::CommandBuffer oneTimeCopy;

  commandBufferOneTimeBegin(oneTimeCopy, commandPool);

  TransitionImage transitionImage;
  transitionImage
    .setCommandBuffer(oneTimeCopy)
    .layouts(vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal)
    .accessMask(vk::AccessFlags(0), vk::AccessFlagBits::eTransferWrite)
    // aspectMask, baseMip, levelCount, baseArray, layerCount
    .subresources({ vk::ImageAspectFlagBits::eColor, 0, mMipmapLevels, 0, 1 })
    .transition(mImage, vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer);

  CopyBuffer copyImage;
  copyImage
    .setCommandBuffer(oneTimeCopy)
    // aspectMask, mipLevel, baseArray, layerCount
    .copyBufferToImage(stageBuffer, mImage, mImageExtent, { vk::ImageAspectFlagBits::eColor, 0, 0, 1 });
    

  TransitionImage transitionImageShaderReady;
  transitionImageShaderReady
    .setCommandBuffer(oneTimeCopy)
    .layouts(vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal)
    .accessMask(vk::AccessFlagBits::eTransferWrite, vk::AccessFlagBits::eShaderRead) 
    .subresources({ vk::ImageAspectFlagBits::eColor, 0, mMipmapLevels, 0, 1 })
    .transition(mImage, vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader);

  commandBufferOneTimeEnd(oneTimeCopy, commandPool, submitQueue);

  Vulkan::device().freeMemory(stageMemory, nullptr);
  Vulkan::device().destroyBuffer(stageBuffer, nullptr);

  vk::ImageViewCreateInfo imageViewInfo;
  imageViewInfo
    .setImage(mImage)
    .setViewType(vk::ImageViewType::e2D)
    .setComponents((vk::ComponentMapping){vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA})
    .setFormat(vk::Format::eR8G8B8A8Srgb)
    .setSubresourceRange((vk::ImageSubresourceRange){vk::ImageAspectFlagBits::eColor, 0, mMipmapLevels, 0, 1});

  if(Vulkan::device().createImageView(&imageViewInfo, nullptr, &mImageView) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create sampler image view");

  vk::PhysicalDeviceProperties gpuProperties;
  Vulkan::gpu().getProperties(&gpuProperties);

  vk::SamplerCreateInfo samplerInfo;
  samplerInfo
    .setAnisotropyEnable(1)
    .setMaxAnisotropy(gpuProperties.limits.maxSamplerAnisotropy)
    .setMipmapMode(vk::SamplerMipmapMode::eLinear)
    .setMinLod(0.0f)
    .setMipLodBias(0.0f)
    .setMaxLod(0.0f)
    .setMinFilter(vk::Filter::eLinear)
    .setMagFilter(vk::Filter::eLinear)
    .setAddressModeU(vk::SamplerAddressMode::eRepeat)
    .setAddressModeV(vk::SamplerAddressMode::eRepeat)
    .setAddressModeW(vk::SamplerAddressMode::eRepeat)
    .setBorderColor(vk::BorderColor::eIntOpaqueBlack)
    .setUnnormalizedCoordinates(0)
    .setCompareEnable(0)
    .setCompareOp(vk::CompareOp::eAlways);

  if (mipLevels > 1)
    samplerInfo.setMaxLod(static_cast<float>(mMipmapLevels));

  if(Vulkan::device().createSampler(&samplerInfo, nullptr, &mSampler) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create sampler sampler");

  return *this;
}

void sf::Sampler::destroy() {
  Vulkan::device().destroySampler(mSampler, nullptr);

  Vulkan::device().destroyImageView(mImageView, nullptr);
  
  Vulkan::device().freeMemory(mImageMemory, nullptr);
  Vulkan::device().destroyImage(mImage, nullptr);
}

sf::ShaderStorageBuffer::~ShaderStorageBuffer() {
  destroy();
}

vk::Buffer& sf::ShaderStorageBuffer::getBuffer() {
  return mBuffer;
}

uint32_t sf::ShaderStorageBuffer::getBufferSize() {
  return mBufferSize;
}

sf::ShaderStorageBuffer& sf::ShaderStorageBuffer::update(void* pData, const size_t size) {
  if (mBufferSize != size || mBuffer == nullptr) {
    destroy();

    mBufferSize = size;

    vk::BufferCreateInfo bufferInfo;
    bufferInfo
      .setSharingMode(vk::SharingMode::eExclusive)
      .setQueueFamilyIndexCount(0)
      .setSize(mBufferSize)
      .setUsage(vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc);

    if(Vulkan::device().createBuffer(&bufferInfo, nullptr, &mBuffer) != vk::Result::eSuccess)
      SF_CLOG("ERR: Cannot create shader storage buffer buffer");

    vk::MemoryAllocateInfo memAllocInfo;
    memAllocInfo
      .setMemoryTypeIndex(findBufferMemoryType(memAllocInfo.allocationSize, mBuffer, vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible));

    if(Vulkan::device().allocateMemory(&memAllocInfo, nullptr, &mBufferMemory) != vk::Result::eSuccess)
      SF_CLOG("ERR: Cannot allocate shader storage buffer memory");

    Vulkan::device().bindBufferMemory(mBuffer, mBufferMemory, 0);
  }

  void* tempMemCopy;
  map(&tempMemCopy);
  memcpy(tempMemCopy, pData, mBufferSize);
  unmap();

  return *this;
}

sf::ShaderStorageBuffer& sf::ShaderStorageBuffer::map(void** ppDataPlace) {
  Vulkan::device().mapMemory(mBufferMemory, 0, mBufferSize, vk::MemoryMapFlags(0), ppDataPlace);

  return *this;
}

sf::ShaderStorageBuffer& sf::ShaderStorageBuffer::unmap() {
  Vulkan::device().unmapMemory(mBufferMemory);

  return *this;
}

void sf::ShaderStorageBuffer::destroy() {
  mBufferSize = 0;

  Vulkan::device().freeMemory(mBufferMemory, nullptr);
  Vulkan::device().destroyBuffer(mBuffer, nullptr);
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

  vk::ShaderModuleCreateInfo shaderModuleInfo;
  shaderModuleInfo
    .setPCode(reinterpret_cast<const uint32_t*>(shaderBinary.data()))
    .setCodeSize(shaderBinary.size());

  if(Vulkan::device().createShaderModule(&shaderModuleInfo, nullptr, &mShaderModule) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create shader module");

  return *this;
}

void sf::Shader::destroy() {
  Vulkan::device().destroyShaderModule(mShaderModule, nullptr);

  mShaderStage = vk::ShaderStageFlags(0);
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
  // Create image
  vk::ImageCreateInfo imageInfo;
  imageInfo
    .setImageType(vk::ImageType::e2D)
    .setArrayLayers(1)
    .setExtent(extent)
    .setFormat(Vulkan::depthFormat())
    .setUsage(vk::ImageUsageFlagBits::eDepthStencilAttachment)
    .setInitialLayout(vk::ImageLayout::eUndefined)
    .setMipLevels(1)
    .setQueueFamilyIndexCount(0)
    .setPQueueFamilyIndices(nullptr)
    .setSamples(vk::SampleCountFlagBits::e1)
    .setSharingMode(vk::SharingMode::eExclusive)
    .setTiling(vk::ImageTiling::eOptimal);

  if(Vulkan::device().createImage(&imageInfo, nullptr, &mDepthImage) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create depth image image");

  // Allocate memory for image
  vk::MemoryAllocateInfo memAllocInfo;
  memAllocInfo
    .setMemoryTypeIndex(findImageMemoryType(memAllocInfo.allocationSize, mDepthImage, vk::MemoryPropertyFlagBits::eDeviceLocal));

  if(Vulkan::device().allocateMemory(&memAllocInfo, nullptr, &mDepthImageMemory) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot allocate depth image mnemory");

  // Bind image to memory
  Vulkan::device().bindImageMemory(mDepthImage, mDepthImageMemory, 0);

  // Create depth image view
  vk::ImageViewCreateInfo imageViewInfo;
  imageViewInfo
    .setImage(mDepthImage)
    .setViewType(vk::ImageViewType::e2D)
    .setComponents((vk::ComponentMapping){vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA})
    .setFormat(imageInfo.format)
    .setSubresourceRange((vk::ImageSubresourceRange){vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1});

  if(Vulkan::device().createImageView(&imageViewInfo, nullptr, &mDepthImageView) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create depth image image view");

  vk::CommandBuffer oneTimeBuffer;

  // Transition image
  commandBufferOneTimeBegin(oneTimeBuffer, pool);

  TransitionImage depthBarrier;
  depthBarrier
    .setCommandBuffer(oneTimeBuffer)
    .layouts(vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal)
    .accessMask(vk::AccessFlags(0), vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite)
    .subresources((vk::ImageSubresourceRange){vk::ImageAspectFlagBits::eDepth, 0,1, 0, 1})
    .transition(mDepthImage, vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eEarlyFragmentTests);

  commandBufferOneTimeEnd(oneTimeBuffer, pool, Vulkan::graphicsQueue());

  return *this;
}

void sf::DepthImage::destroy() {
  Vulkan::device().destroyImageView(mDepthImageView, nullptr);

  Vulkan::device().freeMemory(mDepthImageMemory, nullptr);
  Vulkan::device().destroyImage(mDepthImage, nullptr);
}
