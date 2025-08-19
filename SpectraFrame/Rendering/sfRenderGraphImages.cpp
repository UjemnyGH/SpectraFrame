#include "sfRenderGraphImages.h"
#include "../Core/sfVkCommon.h"
#include "../Core/sfLogger.h"

bool sf::RenderGraphImage::_formatShouldBeSampled(const vk::Format format) const {
  return (
    format == vk::Format::eR8G8B8Uscaled ||
    format == vk::Format::eR8G8B8Sscaled ||
    format == vk::Format::eR8G8B8Snorm ||
    format == vk::Format::eR8G8B8Unorm ||
    format == vk::Format::eR8G8B8Srgb ||
    format == vk::Format::eR8G8B8A8Uscaled ||
    format == vk::Format::eR8G8B8A8Sscaled ||
    format == vk::Format::eR8G8B8A8Snorm ||
    format == vk::Format::eR8G8B8A8Unorm ||
    format == vk::Format::eR8G8B8A8Srgb
  );
}

sf::RenderGraphImage::RenderGraphImage() {
  mSwapchainImageIndex = ~0U;
  mImageSampler = nullptr;
  mImage = nullptr;
  mImageMemory = nullptr;
  mImageView = nullptr;

  mImageCurrentAccessFlags = vk::AccessFlagBits2::eNone;
  mImageCurrentLayout = vk::ImageLayout::eUndefined;
  mImageCurrentStageFlags = vk::PipelineStageFlagBits2::eTopOfPipe;
}

// 
// A lot of getters
//

std::string& sf::RenderGraphImage::getName() {
  return mName; 
}

vk::Image& sf::RenderGraphImage::getImage() {
  return mImage;
}

vk::ImageView& sf::RenderGraphImage::getView() {
  return mImageView;
}

vk::DeviceMemory& sf::RenderGraphImage::getMemory() {
  return mImageMemory;
}

vk::Sampler& sf::RenderGraphImage::getSampler() {
  return mImageSampler;
}

vk::ImageLayout sf::RenderGraphImage::getLayout() const {
  return mImageCurrentLayout;
}

vk::PipelineStageFlags2 sf::RenderGraphImage::getStage() const {
  return mImageCurrentStageFlags;
}

vk::AccessFlags2 sf::RenderGraphImage::getAccess() const {
  return mImageCurrentAccessFlags;
}

vk::Format sf::RenderGraphImage::getFormat() const {
  return mImageFormat;
}

bool sf::RenderGraphImage::isDepth() const {
  return mIsDepthImage;
}

bool sf::RenderGraphImage::isExternal() const {
  return mIsExternal;
}

bool sf::RenderGraphImage::isSampled() const {
  return mIsSampled;
}

uint32_t sf::RenderGraphImage::getSwapchainImageIndex() const {
  return mSwapchainImageIndex;
}

// 
// Create function overload
//

// Create basic image, nothing special
sf::RenderGraphImage& sf::RenderGraphImage::create(const std::string& name, const vk::Format format) {
  // Set base parameters to false
  mIsDepthImage = false;
  mIsExternal = false;

  // Set render pass data
  mName = name;
  mImageFormat = format;

  // Fill image info
  vk::ImageCreateInfo imageInfo;
  imageInfo
    .setImageType(vk::ImageType::e2D)
    .setArrayLayers(1)
    .setExtent(vk::Extent3D(Window::sWindowInstancePtr->getWidth(), Window::sWindowInstancePtr->getHeight(), 1))
    .setFormat(mImageFormat)
    .setInitialLayout(vk::ImageLayout::eUndefined)
    .setMipLevels(1)
    .setQueueFamilyIndexCount(0)
    .setPQueueFamilyIndices(nullptr)
    .setSamples(vk::SampleCountFlagBits::e1)
    .setSharingMode(vk::SharingMode::eExclusive)
    .setTiling(vk::ImageTiling::eOptimal)
    .setUsage(vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled);

  mImageCurrentAccessFlags = vk::AccessFlagBits2::eNone;
  mImageCurrentLayout = vk::ImageLayout::eUndefined;
  mImageCurrentStageFlags = vk::PipelineStageFlagBits2::eTopOfPipe;

  // If it`s depth image then use appropriate to specification
  if(mImageFormat == Vulkan::depthFormat()) {
    imageInfo.setUsage(vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eSampled);

    mIsDepthImage = true;
  }

  if(Vulkan::device().createImage(&imageInfo, nullptr, &mImage) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create render pass image image on " << mName);

  // Allocate memory
  vk::MemoryAllocateInfo memAllocInfo;
  memAllocInfo.setMemoryTypeIndex(findImageMemoryType(memAllocInfo.allocationSize, mImage, vk::MemoryPropertyFlagBits::eDeviceLocal));

  if(Vulkan::device().allocateMemory(&memAllocInfo, nullptr, &mImageMemory) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot allocate render pass image memory on " << mName);

  // Bind image to memory
  Vulkan::device().bindImageMemory(mImage, mImageMemory, 0);

  // Fill image view info struct
  vk::ImageViewCreateInfo imageViewInfo;
  imageViewInfo
    .setImage(mImage)
    .setViewType(vk::ImageViewType::e2D)
    .setComponents(vk::ComponentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA))
    .setFormat(mImageFormat)
    .setSubresourceRange(vk::ImageSubresourceRange(mIsDepthImage ? vk::ImageAspectFlagBits::eDepth : vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

  // Create image view
  if(Vulkan::device().createImageView(&imageViewInfo, nullptr, &mImageView) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create render pass image image view on " << mName);

  mIsSampled = _formatShouldBeSampled(mImageFormat);

  if(!mIsSampled)
    return *this;

  vk::SamplerCreateInfo samplerInfo{};
  samplerInfo
    .setAddressModeU(vk::SamplerAddressMode::eRepeat)
    .setAddressModeV(vk::SamplerAddressMode::eRepeat)
    .setAddressModeW(vk::SamplerAddressMode::eRepeat)
    .setMinFilter(vk::Filter::eLinear)
    .setMagFilter(vk::Filter::eLinear)
    .setBorderColor(vk::BorderColor::eIntOpaqueBlack);

  if(Vulkan::device().createSampler(&samplerInfo, nullptr, &mImageSampler) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create render pass image sampler on " << mName);

  return *this;
}

sf::RenderGraphImage& sf::RenderGraphImage::create(const std::string& name, const vk::Format format, const vk::Image& externalImage) {
  // Here as before
  mIsExternal = true;
  mIsDepthImage = false;

  // Except that we just set out image
  mImage = externalImage;

  mName = name;
  mImageFormat = format;

  mImageCurrentAccessFlags = vk::AccessFlagBits2::eNone;
  mImageCurrentLayout = vk::ImageLayout::eUndefined;
  mImageCurrentStageFlags = vk::PipelineStageFlagBits2::eTopOfPipe;
  
  if(mImageFormat == Vulkan::depthFormat())
    mIsDepthImage = true;

  // And create image view on already existing image
  vk::ImageViewCreateInfo imageViewInfo;
  imageViewInfo
    .setImage(mImage)
    .setViewType(vk::ImageViewType::e2D)
    .setFormat(mImageFormat)
    .setComponents(vk::ComponentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA))
    .setSubresourceRange(vk::ImageSubresourceRange(mIsDepthImage ? vk::ImageAspectFlagBits::eDepth : vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

  if(Vulkan::device().createImageView(&imageViewInfo, nullptr, &mImageView) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create external image render pass image image view");
  
  mIsSampled = _formatShouldBeSampled(mImageFormat);

  if(!mIsSampled)
    return *this;

  vk::SamplerCreateInfo samplerInfo{};
  samplerInfo
    .setAddressModeU(vk::SamplerAddressMode::eRepeat)
    .setAddressModeV(vk::SamplerAddressMode::eRepeat)
    .setAddressModeW(vk::SamplerAddressMode::eRepeat)
    .setMinFilter(vk::Filter::eLinear)
    .setMagFilter(vk::Filter::eLinear)
    .setBorderColor(vk::BorderColor::eIntOpaqueBlack);

  if(Vulkan::device().createSampler(&samplerInfo, nullptr, &mImageSampler) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create render pass image sampler on " << mName);

  return *this;
}

sf::RenderGraphImage& sf::RenderGraphImage::transition(vk::CommandBuffer cmd, const vk::ImageLayout newLayout, const vk::AccessFlags2 dstAccessFlags, const vk::PipelineStageFlags2 dstPipelineStage) {
  if(mImage == nullptr) {
    SF_CLOG("ERR: Image doesnt exist");

    return *this;
  }

  vk::ImageMemoryBarrier2 imageMemoryBarrier{};
  imageMemoryBarrier
    .setSrcAccessMask(mImageCurrentAccessFlags)
    .setSrcStageMask(mImageCurrentStageFlags)
    .setOldLayout(mImageCurrentLayout)
    .setDstAccessMask(dstAccessFlags)
    .setDstStageMask(dstPipelineStage)
    .setNewLayout(newLayout)
    .setImage(mImage)
    .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
    .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
    .setSubresourceRange(vk::ImageSubresourceRange(mIsDepthImage ? vk::ImageAspectFlagBits::eDepth : vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

  vk::DependencyInfo dependencyInfo{};
  dependencyInfo
    .setMemoryBarrierCount(0)
    .setBufferMemoryBarrierCount(0)
    .setPMemoryBarriers(nullptr)
    .setPBufferMemoryBarriers(nullptr)
    .setImageMemoryBarrierCount(1)
    .setPImageMemoryBarriers(&imageMemoryBarrier);

  cmd.pipelineBarrier2(dependencyInfo);

  mImageCurrentAccessFlags = dstAccessFlags;
  mImageCurrentStageFlags = dstPipelineStage;
  mImageCurrentLayout = newLayout;

  return *this;
}

sf::RenderGraphImage& sf::RenderGraphImage::setSwapchainImageIndex(const uint32_t index) {
  // Set index for swapchain images (IMPORTANT TO NOT MESS UP RENDERING!!)
  mSwapchainImageIndex = index;

  return *this;
}

void sf::RenderGraphImage::destroy() {
  if(!mIsExternal) {
    if(mImage)
      Vulkan::device().destroyImage(std::exchange(mImage, nullptr), nullptr);
    if(mImageMemory)
      Vulkan::device().freeMemory(std::exchange(mImageMemory, nullptr), nullptr);
  }

  if(mImageView)
    Vulkan::device().destroyImageView(std::exchange(mImageView, nullptr), nullptr);

  if(mImageSampler)
    Vulkan::device().destroySampler(std::exchange(mImageSampler, nullptr), nullptr);

  mIsExternal = false;
  mIsDepthImage = false;

  mSwapchainImageIndex = ~0U;

  mImageCurrentAccessFlags = vk::AccessFlagBits2::eNone;
  mImageCurrentLayout = vk::ImageLayout::eUndefined;
  mImageCurrentStageFlags = vk::PipelineStageFlagBits2::eTopOfPipe;
}

// Color image, no depth, no swapchain, no external
sf::RenderGraphImageInfo::RenderGraphImageInfo(const std::string& _name, vk::Format _format) {
  name = _name;
  format = _format;
  swapchainImageIndex = ~0U;
  externalImage = nullptr;
}

// Depth image, no swapchain, predefined format, no external
sf::RenderGraphImageInfo::RenderGraphImageInfo(const std::string& _name) {
  name = _name;
  format = Vulkan::depthFormat();
  swapchainImageIndex = ~0U;
  externalImage = nullptr;
}

// External image
sf::RenderGraphImageInfo::RenderGraphImageInfo(const std::string& _name, vk::Format _format, vk::Image _externalImage) {
  name = _name;
  format = _format;
  swapchainImageIndex = ~0U;
  externalImage = _externalImage;
}

// Swapchain image, name them in indexable way
sf::RenderGraphImageInfo::RenderGraphImageInfo(const std::string& _name, vk::Image _externalImage, uint32_t _swapchainImageIndex) {
  name = _name;
  format = Vulkan::surfaceFormat().format;
  swapchainImageIndex = _swapchainImageIndex;
  externalImage = _externalImage;
}

sf::RenderGraphImage& sf::RenderGraphImageManager::operator[](const std::string& name) {
  return *mRenderGraphImagesMapped[name];
}

sf::RenderGraphImage& sf::RenderGraphImageManager::getSwapchainImage(const uint32_t index) {
  return *mSwapchainImages[index];
}

void sf::RenderGraphImageManager::create(const std::vector<RenderGraphImageInfo>& imagesToCreate) {
  // Make temporary swapchain image pointer storage place
  std::deque<RenderGraphImage*> swapchainImagesTemp;

  // Iterate through all images to create
  for(RenderGraphImageInfo info : imagesToCreate) {
    // Add new image
    mRenderGraphImages.emplace_back();
    mRenderGraphImagesMapped[info.name] = &mRenderGraphImages.back();

    // Check if image is external or not, based on that create external image or internal image 
    if(info.externalImage == nullptr)
      mRenderGraphImagesMapped[info.name]->create(info.name, info.format);
    else 
      mRenderGraphImagesMapped[info.name]->create(info.name, info.format, info.externalImage);

    // If there is swapchain image, replace default value (0xFFFFFFFF) with swapchain image index, if not default value would be set again
    mRenderGraphImagesMapped[info.name]->setSwapchainImageIndex(info.swapchainImageIndex);

    // If there is swapchain image add it to temporary swapchain image storage
    if(info.swapchainImageIndex != ~0U)
      swapchainImagesTemp.emplace_back(mRenderGraphImagesMapped[info.name]);
  }

  // Resize swapchain images vector to amount swapchain images that we have
  mSwapchainImages.resize(swapchainImagesTemp.size());

  // Sort swapchain images by indexes
  for(RenderGraphImage* image : swapchainImagesTemp) {
    mSwapchainImages[image->getSwapchainImageIndex()] = image;
  }
}

void sf::RenderGraphImageManager::destroy() {
  mSwapchainImages.clear();
  mRenderGraphImagesMapped.clear();

  for(RenderGraphImage& image : mRenderGraphImages) {
    image.destroy();
  }

  mRenderGraphImages.clear();
}

