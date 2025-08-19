#pragma once
#ifndef _SPECTRAFRAME_RENDER_PASS_IMAGES_
#define _SPECTRAFRAME_RENDER_PASS_IMAGES_

#include "../Core/sfWindow.h"
#include <unordered_map>
#include <deque>

namespace sf {
  class RenderGraphImage {
  private:
    std::string mName;
    vk::Image mImage;
    vk::DeviceMemory mImageMemory;
    vk::ImageView mImageView;
    vk::Sampler mImageSampler;
    vk::Format mImageFormat;

    vk::AccessFlags2 mImageCurrentAccessFlags;
    vk::PipelineStageFlags2 mImageCurrentStageFlags;
    vk::ImageLayout mImageCurrentLayout;

    bool mIsDepthImage;
    bool mIsExternal;
    bool mIsSampled;

    uint32_t mSwapchainImageIndex;

    bool _formatShouldBeSampled(const vk::Format format) const;

  public:
    RenderGraphImage();

    std::string& getName();

    vk::Image& getImage();

    vk::ImageView& getView();

    vk::DeviceMemory& getMemory();

    vk::Sampler& getSampler();

    vk::ImageLayout getLayout() const;

    vk::PipelineStageFlags2 getStage() const;
    
    vk::AccessFlags2 getAccess() const;
  
    vk::Format getFormat() const;

    bool isDepth() const;

    bool isExternal() const;

    bool isSampled() const;

    uint32_t getSwapchainImageIndex() const;

    RenderGraphImage& create(const std::string& name, const vk::Format format);

    RenderGraphImage& create(const std::string& name, const vk::Format format, const vk::Image& externalImage);

    RenderGraphImage& transition(vk::CommandBuffer cmd, const vk::ImageLayout newLayout, const vk::AccessFlags2 dstAccessFlags, const vk::PipelineStageFlags2 dstPipelineStage);

    RenderGraphImage& setSwapchainImageIndex(const uint32_t index);

    void destroy();
  };

  struct RenderGraphImageInfo {
    std::string name;
    vk::Format format;
    uint32_t swapchainImageIndex;
    vk::Image externalImage;

    // Color image, no depth, no swapchain, no external
    RenderGraphImageInfo(const std::string& _name, vk::Format _format);

    // Depth image, no swapchain, predefined format, no external
    RenderGraphImageInfo(const std::string& _name);

    // External image
    RenderGraphImageInfo(const std::string& _name, vk::Format _format, vk::Image _externalImage);

    // Swapchain image, name them in indexable way
    RenderGraphImageInfo(const std::string& _name, vk::Image _externalImage, uint32_t _swapchainImageIndex);
  };

  class RenderGraphImageManager {
  private:
    std::deque<RenderGraphImage> mRenderGraphImages;
    std::unordered_map<std::string, RenderGraphImage*> mRenderGraphImagesMapped;
    std::deque<RenderGraphImage*> mSwapchainImages;

  public:
    RenderGraphImage& operator[](const std::string& name);

    RenderGraphImage& getSwapchainImage(const uint32_t index);

    void create(const std::vector<RenderGraphImageInfo>& imagesToCreate);

    void destroy();
  };
}

#endif
