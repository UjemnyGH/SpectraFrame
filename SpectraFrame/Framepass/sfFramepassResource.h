/*
 * @brief Contents of sfFramepassResource .h/.cpp are only used to store frame rendered images, these are not used to store textures
 *    For textures use resource manager suitable for this usage
 */

#pragma once
#ifndef _SPECTRAFRAME_FRAMEPASS_RESOURCE_
#define _SPECTRAFRAME_FRAMEPASS_RESOURCE_

#include "../Core/sfWindow.h"
#include <unordered_map>

namespace sf {
  /*
   * @brief This struct holds vulkan image data used to render into or read from
   */
  struct FramepassVulkanResource {
    std::string resourceName;

    vk::Image image;
    vk::ImageView imageView;
    vk::DeviceMemory imageMemory;

    vk::ImageUsageFlags usage;
    vk::Format format;
  };

  class FramepassResource {
  private:
    std::vector<FramepassVulkanResource> mFramepassVulkanResource;
    std::unordered_map<std::string, FramepassVulkanResource*> mFramepassVulkanResourceMapped;

    vk::Format _parseImageFormatFromString(std::string& formatString);

  public:
    ~FramepassResource();

    /**
     * This operator can also create new resources 
     * resource name is tagged so if you want to create depth just add to name #depth or #depth@r32f, same for colors: #color@rgb32f or just a format @rgb32f
     * default values are #color@rgba
     * #color (possible values sampled - eSampled, storage - eStorage, depth - eDepthStencilAttachment, color - eColorAttachment, src - as eTransferSrc, dst - as eTransferDst, input - eInputAttachment, host - as eHostTransfer)
     * @rgba32f:
     * rgba - colors used, (possible values r, rg, rgb, rgba, nothing = rgba)
     * 32 - size (possible values 8, 16, 32, nothing = 8)
     * f - type (possible values n - normalized, f - float, i - int, u - unsigned int, s - scaled, nothing - srgb)
     */
    FramepassVulkanResource* operator[](const std::string& resourceName);

    /**
     * Creates new resouurce with desired parameters, image size (width, height) can be left at 0 to create screen size resource
     */
    FramepassResource& create(const std::string& resourceName, const vk::ImageUsageFlags imageUsage, const vk::Format format, const uint32_t imageWidth = 0, const uint32_t imageHeight = 0);
  
    /**
     * Destroys single image
     */
    FramepassResource& destroy(const std::string& resourceName);

    /**
     * Destroys all images
     */
    FramepassResource& destroy();
  };
}

#endif
