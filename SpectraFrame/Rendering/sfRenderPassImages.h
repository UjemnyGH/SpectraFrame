#pragma once
#ifndef _SPECTRAFRAME_RENDER_PASS_IMAGES_
#define _SPECTRAFRAME_RENDER_PASS_IMAGES_

#include "../Core/sfWindow.h"
#include <unordered_map>

namespace sf {
  class RenderPassImage {
  private:
    std::string mName;
    vk::Image mImage;
    vk::DeviceMemory mImageMemory;
    vk::ImageView mImageView;
    vk::ImageLayout mImageFinalLayout;
    vk::Format mImageFormat;

    bool mIsDepthImage;
    bool mIsExternal;

    uint32_t mSwapchainImageIndex;

  public:
    RenderPassImage();

    ~RenderPassImage();

    std::string& getName();

    vk::Image& getImage();

    vk::ImageView& getView();

    vk::DeviceMemory& getMemory();

    vk::ImageLayout& getLayout();

    vk::Format& getFormat();

    bool isDepth();

    bool isExternal();

    uint32_t getSwapchainImageIndex();

    RenderPassImage& create(const std::string& name, const vk::ImageLayout finalLayout, const vk::Format format);

    RenderPassImage& create(const std::string& name, const vk::ImageLayout finalLayout, const vk::Format format, const vk::Image& externalImage);

    RenderPassImage& create(const std::string& name, const vk::ImageLayout finalLayout, const vk::Format format, const vk::ImageView& externalImageView);

    RenderPassImage& setSwapchainImageIndex(uint32_t index);

    void destroy();
  };
}

#endif
