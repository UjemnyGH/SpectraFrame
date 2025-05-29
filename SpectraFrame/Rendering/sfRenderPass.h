#pragma once
#ifndef _SPECTRAFRAME_RENDER_PASS_
#define _SPECTRAFRAME_RENDER_PASS_

#include "../Core/sfWindow.h"

namespace sf {
  struct RenderPassAttachment {
    vk::Format format;
    vk::ImageLayout layout;
  };

  class RenderPass {
  private:
    vk::RenderPass mRenderPass;
    uint32_t mRenderPassId;

    std::vector<RenderPassAttachment> mRenderPassAttachments;

    void _buildVulkanHandles(const std::vector<vk::AttachmentDescription2>& colorAttachments, const std::vector<vk::AttachmentReference2>& colorReferences, const vk::AttachmentReference2& depthReference);

  public:
    RenderPass(uint32_t id);

    ~RenderPass();

    virtual void execute(vk::CommandBuffer& cmd) {};

    void build();

    void destroy();

    void run(vk::CommandBuffer& cmd, vk::Framebuffer& framebuffer);

    RenderPass& addAttachment(vk::Format format, vk::ImageLayout layout);

    void clearAttachments();

    uint32_t getId() const;

    vk::RenderPass& getRenderPass();
  };
}

#endif
