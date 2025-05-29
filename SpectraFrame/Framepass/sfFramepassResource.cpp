#include "sfFramepassResource.h"
#include "Core/sfVkCommon.h"

vk::Format sf::FramepassResource::_parseImageFormatFromString(std::string& formatString) {
  uint32_t channelAmonut = 0xf000;
  uint32_t size = 0x8;
  uint32_t type = 0;

  if(     formatString.find("rgba") < formatString.size()) channelAmonut = 0xf000;
  else if(formatString.find("rgb")  < formatString.size()) channelAmonut = 0x7000;
  else if(formatString.find("rg")   < formatString.size()) channelAmonut = 0x3000;
  else if(formatString.find("r")    < formatString.size()) channelAmonut = 0x1000;

  if(     formatString.find("8")    < formatString.size()) size = 0x8;
  else if(formatString.find("16")   < formatString.size()) size = 0x10;
  else if(formatString.find("32")   < formatString.size()) size = 0x20;
  else if(formatString.find("64")   < formatString.size()) size = 0x40;

  if(     formatString.find("n")    < formatString.size()) type = 0x1;
  else if(formatString.find("f")    < formatString.size()) type = 0x2;
  else if(formatString.find("i")    < formatString.size()) type = 0x4;
  else if(formatString.find("u")    < formatString.size()) type = 0x80;
  else if(formatString.find("s")    < formatString.size()) type = 0x100;
  
  // ChannelAmount, Size, Type
  const std::unordered_map<uint32_t, vk::Format> formats = {
    {0x1000 | 0x8 | 0x0, vk::Format::eR8Srgb},
    {0x3000 | 0x8 | 0x0, vk::Format::eR8G8Srgb},
    {0x7000 | 0x8 | 0x0, vk::Format::eR8G8B8Srgb},
    {0xf000 | 0x8 | 0x0, vk::Format::eR8G8B8A8Srgb},

    {0x1000 | 0x8 | 0x1, vk::Format::eR8Snorm},
    {0x3000 | 0x8 | 0x1, vk::Format::eR8G8Snorm},
    {0x7000 | 0x8 | 0x1, vk::Format::eR8G8B8Snorm},
    {0xf000 | 0x8 | 0x1, vk::Format::eR8G8B8A8Snorm},
    
    {0x1000 | 0x8 | 0x4, vk::Format::eR8Sint},
    {0x3000 | 0x8 | 0x4, vk::Format::eR8G8Sint},
    {0x7000 | 0x8 | 0x4, vk::Format::eR8G8B8Sint},
    {0xf000 | 0x8 | 0x4, vk::Format::eR8G8B8A8Sint},

    {0x1000 | 0x8 | 0x80, vk::Format::eR8Uint},
    {0x3000 | 0x8 | 0x80, vk::Format::eR8G8Uint},
    {0x7000 | 0x8 | 0x80, vk::Format::eR8G8B8Uint},
    {0xf000 | 0x8 | 0x80, vk::Format::eR8G8B8A8Uint},
    
    {0x1000 | 0x8 | 0x100, vk::Format::eR8Sscaled},
    {0x3000 | 0x8 | 0x100, vk::Format::eR8G8Sscaled},
    {0x7000 | 0x8 | 0x100, vk::Format::eR8G8B8Sscaled},
    {0xf000 | 0x8 | 0x100, vk::Format::eR8G8B8A8Sscaled},
    
    {0x1000 | 0x10 | 0x1, vk::Format::eR16Snorm},
    {0x3000 | 0x10 | 0x1, vk::Format::eR16G16Snorm},
    {0x7000 | 0x10 | 0x1, vk::Format::eR16G16B16Snorm},
    {0xf000 | 0x10 | 0x1, vk::Format::eR16G16B16A16Snorm},
    
    {0x1000 | 0x10 | 0x2, vk::Format::eR16Sfloat},
    {0x3000 | 0x10 | 0x2, vk::Format::eR16G16Sfloat},
    {0x7000 | 0x10 | 0x2, vk::Format::eR16G16B16Sfloat},
    {0xf000 | 0x10 | 0x2, vk::Format::eR16G16B16A16Sfloat},
    
    {0x1000 | 0x10 | 0x4, vk::Format::eR16Sint},
    {0x3000 | 0x10 | 0x4, vk::Format::eR16G16Sint},
    {0x7000 | 0x10 | 0x4, vk::Format::eR16G16B16Sint},
    {0xf000 | 0x10 | 0x4, vk::Format::eR16G16B16A16Sint},
    
    {0x1000 | 0x10 | 0x80, vk::Format::eR16Uint},
    {0x3000 | 0x10 | 0x80, vk::Format::eR16G16Uint},
    {0x7000 | 0x10 | 0x80, vk::Format::eR16G16B16Uint},
    {0xf000 | 0x10 | 0x80, vk::Format::eR16G16B16A16Uint},

    {0x1000 | 0x10 | 0x100, vk::Format::eR16Sscaled},
    {0x3000 | 0x10 | 0x100, vk::Format::eR16G16Sscaled},
    {0x7000 | 0x10 | 0x100, vk::Format::eR16G16B16Sscaled},
    {0xf000 | 0x10 | 0x100, vk::Format::eR16G16B16A16Sscaled},

    {0x1000 | 0x20 | 0x2, vk::Format::eR32Sfloat},
    {0x3000 | 0x20 | 0x2, vk::Format::eR32G32Sfloat},
    {0x7000 | 0x20 | 0x2, vk::Format::eR32G32B32Sfloat},
    {0xf000 | 0x20 | 0x2, vk::Format::eR32G32B32A32Sfloat},
    
    {0x1000 | 0x20 | 0x4, vk::Format::eR32Sint},
    {0x3000 | 0x20 | 0x4, vk::Format::eR32G32Sint},
    {0x7000 | 0x20 | 0x4, vk::Format::eR32G32B32Sint},
    {0xf000 | 0x20 | 0x4, vk::Format::eR32G32B32A32Sint},
    
    {0x1000 | 0x20 | 0x80, vk::Format::eR32Uint},
    {0x3000 | 0x20 | 0x80, vk::Format::eR32G32Uint},
    {0x7000 | 0x20 | 0x80, vk::Format::eR32G32B32Uint},
    {0xf000 | 0x20 | 0x80, vk::Format::eR32G32B32A32Uint},

    {0x1000 | 0x40 | 0x2, vk::Format::eR64Sfloat},
    {0x3000 | 0x40 | 0x2, vk::Format::eR64G64Sfloat},
    {0x7000 | 0x40 | 0x2, vk::Format::eR64G64B64Sfloat},
    {0xf000 | 0x40 | 0x2, vk::Format::eR64G64B64A64Sfloat},
    
    {0x1000 | 0x40 | 0x4, vk::Format::eR64Sint},
    {0x3000 | 0x40 | 0x4, vk::Format::eR64G64Sint},
    {0x7000 | 0x40 | 0x4, vk::Format::eR64G64B64Sint},
    {0xf000 | 0x40 | 0x4, vk::Format::eR64G64B64A64Sint},
    
    {0x1000 | 0x40 | 0x80, vk::Format::eR64Uint},
    {0x3000 | 0x40 | 0x80, vk::Format::eR64G64Uint},
    {0x7000 | 0x40 | 0x80, vk::Format::eR64G64B64Uint},
    {0xf000 | 0x40 | 0x80, vk::Format::eR64G64B64A64Uint}
  };

  return formats.find(channelAmonut | size | type)->second;
}

sf::FramepassResource::~FramepassResource() {
  destroy();
}

// resource name is tagged so if you want to create depth just add to name #depth or #depth@r32f, same for colors: #color@rgb32f or just a format @rgb32f
// default values are #color@rgba
// #color (possible values sampled - eSampled, storage - eStorage, depth - eDepthStencilAttachment, color - eColorAttachment, src - as eTransferSrc, dst - as eTransferDst, input - eInputAttachment, host - as eHostTransfer)
// @rgba32f:
// rgba - colors used, (possible values r, rg, rgb, rgba, nothing = rgba)
// 32 - size (possible values 8, 16, 32, nothing = 8)
// f - type (possible values n - normalized, f - float, i - int, u - unsigned int, s - scaled, nothing - srgb)
sf::FramepassVulkanResource* sf::FramepassResource::operator[](const std::string& resourceName) {
  if(mFramepassVulkanResourceMapped.find(resourceName) != mFramepassVulkanResourceMapped.end())
    return mFramepassVulkanResourceMapped[resourceName];

  size_t imageUsageName = resourceName.find("#");
  size_t imageFormatName = resourceName.find("@");

  vk::ImageUsageFlags imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
  vk::Format imageFormat = vk::Format::eR8G8B8A8Srgb;

  if(imageUsageName < resourceName.size()) {
    std::string usage(resourceName.begin() + imageUsageName, resourceName.end());

    if(     usage.find("sampled")  < usage.size()) imageUsage = vk::ImageUsageFlagBits::eSampled;
    else if(usage.find("storage")  < usage.size()) imageUsage = vk::ImageUsageFlagBits::eStorage;
    else if(usage.find("depth")    < usage.size()) imageUsage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
    else if(usage.find("color")    < usage.size()) imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
    else if(usage.find("src")      < usage.size()) imageUsage = vk::ImageUsageFlagBits::eTransferSrc;
    else if(usage.find("dst")      < usage.size()) imageUsage = vk::ImageUsageFlagBits::eTransferDst;
    else if(usage.find("input")    < usage.size()) imageUsage = vk::ImageUsageFlagBits::eInputAttachment;
    else if(usage.find("host")     < usage.size()) imageUsage = vk::ImageUsageFlagBits::eHostTransfer;
  }

  if(imageFormatName < resourceName.size()) {
    std::string format(resourceName.begin() + imageFormatName, resourceName.end());

    imageFormat = _parseImageFormatFromString(format);
  }

  create(resourceName, imageUsage, imageFormat);

  return mFramepassVulkanResourceMapped[resourceName];
}

sf::FramepassResource& sf::FramepassResource::create(const std::string& resourceName, const vk::ImageUsageFlags imageUsage, const vk::Format format, const uint32_t imageWidth, const uint32_t imageHeight) {
  bool resourceExist = false;

  // If resource with such name exist, destroy it
  if(mFramepassVulkanResourceMapped.find(resourceName) != mFramepassVulkanResourceMapped.end()) {
    // Destroy vulkan part of resource
    Vulkan::device().destroyImage(mFramepassVulkanResourceMapped[resourceName]->image);
    Vulkan::device().destroyImageView(mFramepassVulkanResourceMapped[resourceName]->imageView);

    resourceExist = true;
  }

  // Check if such a resource exist, if not add it
  if(!resourceExist) {
    mFramepassVulkanResource.push_back((FramepassVulkanResource){.resourceName = resourceName});
    mFramepassVulkanResourceMapped[resourceName] = &mFramepassVulkanResource[mFramepassVulkanResource.size() - 1];
  }

  // Change image usage and image format
  mFramepassVulkanResourceMapped[resourceName]->format = format;
  mFramepassVulkanResourceMapped[resourceName]->usage = imageUsage;

  // Fill up image create info 
  vk::ImageCreateInfo imageInfo;
  imageInfo
    .setImageType(vk::ImageType::e2D)
    .setArrayLayers(1)
    .setMipLevels(1)
    .setFormat(mFramepassVulkanResourceMapped[resourceName]->format)
    .setTiling(vk::ImageTiling::eOptimal)
    .setInitialLayout(vk::ImageLayout::eUndefined)
    .setUsage(mFramepassVulkanResourceMapped[resourceName]->usage)
    .setSharingMode(vk::SharingMode::eExclusive)
    .setSamples(vk::SampleCountFlagBits::e1)
    .setExtent(vk::Extent3D(imageWidth, imageHeight, 1));

  // Check if there is declared height and width, if not use window size
  if(imageWidth == 0 || imageHeight == 0)
    imageInfo.setExtent(vk::Extent3D(Window::sWindowInstancePtr->getWidth(), Window::sWindowInstancePtr->getHeight(), 1));

  // Create image
  Vulkan::device().createImage(&imageInfo, nullptr, &mFramepassVulkanResourceMapped[resourceName]->image);

  // Fill memory allocation info for image 
  vk::MemoryAllocateInfo memAllocInfo;
  memAllocInfo
    .setMemoryTypeIndex(findImageMemoryType(memAllocInfo.allocationSize, mFramepassVulkanResourceMapped[resourceName]->image, vk::MemoryPropertyFlagBits::eDeviceLocal));

  // Allocate mmeory for image
  Vulkan::device().allocateMemory(&memAllocInfo, nullptr, &mFramepassVulkanResourceMapped[resourceName]->imageMemory);

  // Bind memory with image
  Vulkan::device().bindImageMemory(mFramepassVulkanResourceMapped[resourceName]->image, mFramepassVulkanResourceMapped[resourceName]->imageMemory, 0);

  vk::ImageViewCreateInfo imageViewInfo;
  imageViewInfo
    .setImage(mFramepassVulkanResourceMapped[resourceName]->image)
    .setViewType(vk::ImageViewType::e2D)
    .setFormat(mFramepassVulkanResourceMapped[resourceName]->format);

  imageViewInfo.subresourceRange
    .setAspectMask(mFramepassVulkanResourceMapped[resourceName]->usage == vk::ImageUsageFlagBits::eDepthStencilAttachment ? vk::ImageAspectFlagBits::eDepth : vk::ImageAspectFlagBits::eColor)
    .setBaseArrayLayer(0)
    .setBaseMipLevel(0)
    .setLayerCount(1)
    .setLevelCount(1);

  imageViewInfo.components
    .setR(vk::ComponentSwizzle::eR)
    .setG(vk::ComponentSwizzle::eG)
    .setB(vk::ComponentSwizzle::eB)
    .setA(vk::ComponentSwizzle::eA);

  Vulkan::device().createImageView(&imageViewInfo, nullptr, &mFramepassVulkanResourceMapped[resourceName]->imageView);

  return *this;
}

sf::FramepassResource& sf::FramepassResource::destroy(const std::string& resourceName) {
  Vulkan::device().destroyImageView(mFramepassVulkanResourceMapped[resourceName]->imageView, nullptr);
  Vulkan::device().destroyImage(mFramepassVulkanResourceMapped[resourceName]->image, nullptr);
  Vulkan::device().freeMemory(mFramepassVulkanResourceMapped[resourceName]->imageMemory, nullptr);

  std::vector<FramepassVulkanResource>::iterator iter = std::vector<FramepassVulkanResource>::iterator(mFramepassVulkanResourceMapped[resourceName]);

  mFramepassVulkanResourceMapped.erase(resourceName);
  mFramepassVulkanResource.erase(iter);

  return *this;
}

sf::FramepassResource& sf::FramepassResource::destroy() {
  mFramepassVulkanResourceMapped.clear();

  for(FramepassVulkanResource& res : mFramepassVulkanResource) {
    Vulkan::device().destroyImageView(res.imageView, nullptr);
    Vulkan::device().destroyImage(res.image, nullptr);
    Vulkan::device().freeMemory(res.imageMemory, nullptr);
  }

  mFramepassVulkanResource.clear();

  return *this;
}
