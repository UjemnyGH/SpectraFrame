#include "sfRenderPass.h"
#include "../Core/sfLogger.h"

void sf::RenderPass::_buildVulkanHandles(const std::vector<vk::AttachmentDescription2>& colorAttachments, const std::vector<vk::AttachmentReference2>& colorReferences, const vk::AttachmentReference2& depthReference) {
  vk::SubpassDescription2 subpassDescription;
  subpassDescription
    .setColorAttachmentCount(static_cast<uint32_t>(colorReferences.size()))
    .setPColorAttachments(colorReferences.data())
    .setPDepthStencilAttachment(&depthReference)
    .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);

  vk::SubpassDependency2 subpassDependency;
  subpassDependency
    .setSrcSubpass(VK_SUBPASS_EXTERNAL)
    .setDstSubpass(0)
    .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests)
    .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests)
    .setSrcAccessMask(vk::AccessFlags(0))
    .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite);

  vk::RenderPassCreateInfo2 renderPassInfo;
  renderPassInfo
    .setAttachmentCount(static_cast<uint32_t>(colorAttachments.size()))
    .setPAttachments(colorAttachments.data())
    .setSubpassCount(1)
    .setPSubpasses(&subpassDescription)
    .setDependencyCount(1)
    .setPDependencies(&subpassDependency);

  // Create render pass before framebuffer as framebuffer needs to know which render pass it is using
  if(Vulkan::device().createRenderPass2(&renderPassInfo, nullptr, &mRenderPass) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create render pass 2");
}

sf::RenderPass::RenderPass(uint32_t id) {
  mRenderPassId = id;
}

sf::RenderPass::~RenderPass() {
  // Clear and destroy render pass 2
  destroy();
}

void sf::RenderPass::build() {
  // Reserve space for attachments
  std::vector<vk::AttachmentDescription2> colorAttachments;
  vk::AttachmentDescription2 depthAttachment;
  colorAttachments.reserve(mRenderPassAttachments.size() - 1);

  // Reserve space for references
  std::vector<vk::AttachmentReference2> colorReferences;
  vk::AttachmentReference2 depthReference;
  colorReferences.reserve(mRenderPassAttachments.size() - 1);

  // All image view attachments for framebuffer
  std::vector<vk::ImageView> framebufferAttachments;
  framebufferAttachments.reserve(mRenderPassAttachments.size());

  // Attachment iterator (needed for ids for references)
  uint32_t attachmentIter = 0;

  // Iterate through all attachments
  for(const RenderPassAttachment attachment : mRenderPassAttachments) {
    // Make default attachment description
    vk::AttachmentDescription2 desc;
    desc
      .setFormat(attachment.format)
      .setFinalLayout(attachment.layout)
      .setInitialLayout(vk::ImageLayout::eUndefined)
      .setLoadOp(vk::AttachmentLoadOp::eClear)
      .setStoreOp(vk::AttachmentStoreOp::eStore)
      .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
      .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
      .setSamples(vk::SampleCountFlagBits::e1);

    // Make default attachment reference
    vk::AttachmentReference2 ref;
    ref
      .setAspectMask(vk::ImageAspectFlagBits::eColor)
      .setLayout(attachment.layout);

    if(attachment.layout == vk::ImageLayout::ePresentSrcKHR)
      ref.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

    // If format is depth, that means that we have depth image
    if(attachment.format == Vulkan::depthFormat()) {
      // In depth store should be set to eDontCare
      desc.setStoreOp(vk::AttachmentStoreOp::eDontCare);

      depthAttachment = desc;

      // In depth aspect needs to be changed to eDepth
      ref.setAspectMask(vk::ImageAspectFlagBits::eDepth);

      depthReference = ref;
    }
    else {
      // If it`s color attachment, just push it
      colorAttachments.emplace_back(desc);

      // Add attachment id to attachment reference
      ref.setAttachment(attachmentIter);

      colorReferences.emplace_back(ref);

      attachmentIter++;
    }
  }

  // Set depth attachment id as last
  depthReference.setAttachment(attachmentIter);

  // Add depth attachment at the end
  colorAttachments.emplace_back(depthAttachment);

  _buildVulkanHandles(colorAttachments, colorReferences, depthReference);
}

void sf::RenderPass::destroy() {
  clearAttachments();

  Vulkan::device().destroyRenderPass(mRenderPass, nullptr);
}

void sf::RenderPass::run(vk::CommandBuffer& cmd, vk::Framebuffer& framebuffer) {
  std::array<vk::ClearValue, 2> clearValues;
  clearValues[0].setColor(vk::ClearColorValue(0.1f, 0.1f, 0.1f, 1.0f));
  clearValues[1].setDepthStencil(vk::ClearDepthStencilValue(1.0f, 0));

  vk::RenderPassBeginInfo beginInfo;
  beginInfo
    .setRenderPass(mRenderPass)
    .setFramebuffer(framebuffer)
    .setRenderArea(vk::Rect2D({0, 0}, {Window::sWindowInstancePtr->getWidth(), Window::sWindowInstancePtr->getHeight()}))
    .setClearValueCount(static_cast<uint32_t>(clearValues.size()))
    .setPClearValues(clearValues.data());

  vk::SubpassBeginInfo subpassBeginInfo;
  subpassBeginInfo
    .setContents(vk::SubpassContents::eInline);

  vk::SubpassEndInfo subpassEndInfo;

  cmd.beginRenderPass2(&beginInfo, &subpassBeginInfo);
  execute(cmd);
  cmd.endRenderPass2(&subpassEndInfo);
}

sf::RenderPass& sf::RenderPass::addAttachment(vk::Format format, vk::ImageLayout layout) {
  mRenderPassAttachments.push_back((RenderPassAttachment){format, layout});

  return *this;
}

void sf::RenderPass::clearAttachments() {
  mRenderPassAttachments.clear();
}

uint32_t sf::RenderPass::getId() const {
  return mRenderPassId;
}

vk::RenderPass& sf::RenderPass::getRenderPass() {
  return mRenderPass;
}
