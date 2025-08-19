#pragma once
#ifndef _SPECTRAFRAME_RENDER_PASS_
#define _SPECTRAFRAME_RENDER_PASS_

#include "../Core/sfWindow.h"
#include <functional>
#include "sfRenderGraphImages.h"

namespace sf {
  /*struct RenderGraphAttachment {
    vk::Format format;
    vk::ImageLayout layout;
    vk::ImageLayout finalLayout;
  };

  class RenderGraphPass {
  private:
    vk::RenderPass mRenderPass;
    std::string mName;

    std::vector<RenderGraphAttachment> mRenderPassAttachments;

    void _buildVulkanHandles(const std::vector<vk::AttachmentDescription2>& colorAttachments, const std::vector<vk::AttachmentReference2>& colorReferences, const vk::AttachmentReference2& depthReference, const bool depthExist);

  public:
    std::function<void(vk::CommandBuffer&)> execute;

    RenderGraphPass(const std::string& name);

    ~RenderGraphPass();

    void build();

    void destroy();

    void run(vk::CommandBuffer& cmd, vk::Framebuffer& framebuffer);

    RenderGraphPass& addAttachment(const vk::Format format, const vk::ImageLayout layout, const vk::ImageLayout finalLayout);

    void clearAttachments();

    const std::string& getName() const;

    vk::RenderPass& getRenderPass();
  };*/

  struct RenderGraphManagerAttachment {
    std::string name;
    std::function<void(vk::CommandBuffer&, RenderGraphImageManager&)> execute;

    std::vector<std::string> writeImageNames;
    std::string depthImageName;
  };

  class RenderGraphManager : public ThreadWorker {
  private:
    struct RenderGraphBakedAttachmentInfo {
      std::string name;
      uint32_t swapchainColorAttachmentIndex;
      std::vector<vk::RenderingAttachmentInfo> colorAttachmentInfos;
      vk::RenderingAttachmentInfo depthAttachmentInfo;
    };

    std::deque<RenderGraphManagerAttachment> mRenderGraphAttachments;
    std::unordered_map<std::string, RenderGraphManagerAttachment*> mRenderGraphAttachmentsMapped;

    std::deque<RenderGraphBakedAttachmentInfo> mBakedAttachments;
    std::unordered_map<std::string, RenderGraphBakedAttachmentInfo*> mBakedAttachmentsMapped;

    RenderGraphImageManager mRenderGraphImageManager;

    std::vector<RenderGraphImageInfo> mImagesInfo;

  public:
    RenderGraphManager();

    RenderGraphManager& addAttachment(RenderGraphManagerAttachment attachment);

    RenderGraphManager& addImage(const std::string& name, const vk::Format format);

    RenderGraphManager& addDepthImage(const std::string& name);

    RenderGraphManager& addSwapchainImage(const std::string& name, vk::Image swapchainImage, const uint32_t imageIndex);

    RenderGraphManager& addExternalImage(const std::string& name, vk::Image image, const vk::Format format);

    RenderGraphManager& build();

    RenderGraphManager& rebuild();

    RenderGraphManager& setThreadWorkerRebuild();

    RenderGraphManager& setThreadWorkerBuild();

    RenderGraphManager& run(vk::CommandBuffer& cmd, const std::string& name);

    RenderGraphImageManager& getImageManager();

    void destroy();
  };
}

#endif
