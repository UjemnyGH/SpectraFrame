#include "sfRenderPassImages.h"
#include "../Core/sfVkCommon.h"

sf::RenderPassImage::RenderPassImage() {
  mSwapchainImageIndex = ~0U;
}

sf::RenderPassImage::~RenderPassImage() {
  destroy();
}

// 
// A lot of getters
//

std::string& sf::RenderPassImage::getName() {
  return mName; 
}

vk::Image& sf::RenderPassImage::getImage() {
  return mImage;
}

vk::ImageView& sf::RenderPassImage::getView() {
  return mImageView;
}

vk::DeviceMemory& sf::RenderPassImage::getMemory() {
  return mImageMemory;
}

vk::ImageLayout& sf::RenderPassImage::getLayout() {
  return mImageFinalLayout;
}

vk::Format& sf::RenderPassImage::getFormat() {
  return mImageFormat;
}

bool sf::RenderPassImage::isDepth() {
  return mIsDepthImage;
}

bool sf::RenderPassImage::isExternal() {
  return mIsExternal;
}

uint32_t sf::RenderPassImage::getSwapchainImageIndex() {
  return mSwapchainImageIndex;
}

// 
// Create function overload
//

// Create basic image, nothing special
sf::RenderPassImage& sf::RenderPassImage::create(const std::string& name, const vk::ImageLayout finalLayout, const vk::Format format) {
  // Set base parameters to false
  mIsDepthImage = false;
  mIsExternal = false;

  // Set render pass data
  mName = name;
  mImageFinalLayout = finalLayout;
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
    .setUsage(vk::ImageUsageFlagBits::eColorAttachment);

  // If it`s depth image then use appropriate to specification
  if(mImageFormat == Vulkan::depthFormat()) {
    imageInfo.setUsage(vk::ImageUsageFlagBits::eDepthStencilAttachment);
  
    mIsDepthImage = true;
  }

  Vulkan::device().createImage(&imageInfo, nullptr, &mImage);

  // Allocate memory
  vk::MemoryAllocateInfo memAllocInfo;
  memAllocInfo.setMemoryTypeIndex(findImageMemoryType(memAllocInfo.allocationSize, mImage, vk::MemoryPropertyFlagBits::eDeviceLocal));

  Vulkan::device().allocateMemory(&memAllocInfo, nullptr, &mImageMemory);

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
  Vulkan::device().createImageView(&imageViewInfo, nullptr, &mImageView);

  return *this;
}

sf::RenderPassImage& sf::RenderPassImage::create(const std::string& name, const vk::ImageLayout finalLayout, const vk::Format format, const vk::Image& externalImage) {
  // Here as before
  mIsExternal = true;
  mIsDepthImage = false;

  // Except that we just set out image
  mImage = externalImage;

  mName = name;
  mImageFinalLayout = finalLayout;
  mImageFormat = format;

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

  Vulkan::device().createImageView(&imageViewInfo, nullptr, &mImageView);

  return *this;
}

sf::RenderPassImage& sf::RenderPassImage::create(const std::string& name, const vk::ImageLayout finalLayout, const vk::Format format, const vk::ImageView& externalImageView) {
  mIsExternal = true;
  mIsDepthImage = false;

  // Here we dont even bother creating image view as we have it externally
  mImageView = externalImageView;

  mName = name;
  mImageFinalLayout = finalLayout;
  mImageFormat = format;

  if(mImageFormat == Vulkan::depthFormat())
    mIsDepthImage = true;

  return *this;
}

sf::RenderPassImage& sf::RenderPassImage::setSwapchainImageIndex(uint32_t index) {
  // Set index for swapchain images (IMPORTANT TO NOT MESS UP RENDERING!!)
  mSwapchainImageIndex = index;

  return *this;
}

void sf::RenderPassImage::destroy() {
  if(!mIsExternal) {
    Vulkan::device().destroyImage(mImage, nullptr);
    Vulkan::device().freeMemory(mImageMemory, nullptr);
  }

  Vulkan::device().destroyImageView(mImageView, nullptr);

  mIsExternal = false;
  mIsDepthImage = false;

  mImageFinalLayout = vk::ImageLayout::eUndefined;
}

