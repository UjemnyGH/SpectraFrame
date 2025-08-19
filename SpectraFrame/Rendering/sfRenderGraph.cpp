#include "sfRenderGraph.h"
#include "../Core/sfLogger.h"

/*void sf::RenderPass::_buildVulkanHandles(const std::vector<vk::AttachmentDescription2>& colorAttachments, const std::vector<vk::AttachmentReference2>& colorReferences, const vk::AttachmentReference2& depthReference, const bool depthExist) {
  vk::SubpassDescription2 subpassDescription;
  subpassDescription
    .setColorAttachmentCount(static_cast<uint32_t>(colorReferences.size()))
    .setPColorAttachments(colorReferences.data())
    .setPDepthStencilAttachment(depthExist ? &depthReference : nullptr)
    .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);

  vk::SubpassDependency2 subpassDependency;
  subpassDependency
    .setSrcSubpass(VK_SUBPASS_EXTERNAL)
    .setDstSubpass(0)
    .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests)
    .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests)
    .setSrcAccessMask(vk::AccessFlags(0))
    .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite | (depthExist ? vk::AccessFlagBits::eDepthStencilAttachmentWrite : vk::AccessFlags(0)));

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

sf::RenderPass::RenderPass(const std::string& name) : mName(name) {
  
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
  bool depthExist = false;

  // Iterate through all attachments
  for(const RenderPassAttachment attachment : mRenderPassAttachments) {
    // Make default attachment description
    vk::AttachmentDescription2 desc;
    desc
      .setFormat(attachment.format)
      .setFinalLayout(attachment.finalLayout)
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

    // If format is depth, that means that we have depth image
    if(attachment.format == Vulkan::depthFormat()) {
      // In depth store should be set to eDontCare
      desc.setStoreOp(vk::AttachmentStoreOp::eDontCare);

      depthAttachment = desc;

      // In depth aspect needs to be changed to eDepth
      ref.setAspectMask(vk::ImageAspectFlagBits::eDepth);

      depthReference = ref;

      depthExist = true;
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

  if(depthExist) {
    // Set depth attachment id as last
    depthReference.setAttachment(attachmentIter);

    // Add depth attachment at the end
    colorAttachments.emplace_back(depthAttachment);
  }

  _buildVulkanHandles(colorAttachments, colorReferences, depthReference, depthExist);
}

void sf::RenderPass::destroy() {
  clearAttachments();
  
  if(mRenderPass)
    Vulkan::device().destroyRenderPass(std::exchange(mRenderPass, nullptr), nullptr);
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

sf::RenderPass& sf::RenderPass::addAttachment(const vk::Format format, const vk::ImageLayout layout, const vk::ImageLayout finalLayout) {
  mRenderPassAttachments.push_back((RenderPassAttachment){format, layout, finalLayout});

  return *this;
}

void sf::RenderPass::clearAttachments() {
  mRenderPassAttachments.clear();
}

const std::string& sf::RenderPass::getName() const {
  return mName;
}

vk::RenderPass& sf::RenderPass::getRenderPass() {
  return mRenderPass;
}*/

sf::RenderGraphManager::RenderGraphManager() : ThreadWorker(5) {
  executeFunction = [this]() { build(); };
}

sf::RenderGraphManager& sf::RenderGraphManager::addAttachment(RenderGraphManagerAttachment attachment) {
  mRenderGraphAttachments.emplace_back(attachment);
  mRenderGraphAttachmentsMapped[attachment.name] = &mRenderGraphAttachments.back();

  return *this;
}

sf::RenderGraphManager& sf::RenderGraphManager::addImage(const std::string& name, const vk::Format format) {
  mImagesInfo.emplace_back(RenderGraphImageInfo(name, format));

  return *this;
}

sf::RenderGraphManager& sf::RenderGraphManager::addDepthImage(const std::string& name) {
  mImagesInfo.emplace_back(RenderGraphImageInfo(name));

  return *this;
}

sf::RenderGraphManager& sf::RenderGraphManager::addSwapchainImage(const std::string& name, vk::Image swapchainImage, const uint32_t imageIndex) {
  mImagesInfo.emplace_back(RenderGraphImageInfo(name, swapchainImage, imageIndex));

  return *this;
}

sf::RenderGraphManager& sf::RenderGraphManager::addExternalImage(const std::string& name, vk::Image image, const vk::Format format) {
  mImagesInfo.emplace_back(RenderGraphImageInfo(name, format, image));

  return *this;
}

sf::RenderGraphManager& sf::RenderGraphManager::build() {
  mRenderGraphImageManager.create(mImagesInfo);

  for(RenderGraphManagerAttachment& attachment : mRenderGraphAttachments) {
    mBakedAttachments.emplace_back(RenderGraphBakedAttachmentInfo{attachment.name, ~0U });
    mBakedAttachments.back().colorAttachmentInfos.reserve(attachment.writeImageNames.size());
    mBakedAttachmentsMapped[attachment.name] = &mBakedAttachments.back();

    if(!attachment.depthImageName.empty()) {
      mBakedAttachmentsMapped[attachment.name]->depthAttachmentInfo
        .setImageLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
        .setImageView(mRenderGraphImageManager[attachment.depthImageName].getView())
        .setClearValue(vk::ClearValue(vk::ClearDepthStencilValue(1.0f, 0)))
        .setLoadOp(vk::AttachmentLoadOp::eClear)
        .setStoreOp(vk::AttachmentStoreOp::eDontCare);
    }

    for(const std::string& write : attachment.writeImageNames) {
      mBakedAttachmentsMapped[attachment.name]->colorAttachmentInfos.emplace_back(vk::RenderingAttachmentInfo{});
      mBakedAttachmentsMapped[attachment.name]->colorAttachmentInfos[mBakedAttachmentsMapped[attachment.name]->colorAttachmentInfos.size() - 1]
        .setImageLayout(vk::ImageLayout::eColorAttachmentOptimal)
        .setImageView(mRenderGraphImageManager[write].getView())
        .setClearValue(vk::ClearValue(vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f)))
        .setLoadOp(vk::AttachmentLoadOp::eClear)
        .setStoreOp(vk::AttachmentStoreOp::eStore);

      if(mRenderGraphImageManager[write].getSwapchainImageIndex() != ~0U)
        mBakedAttachmentsMapped[attachment.name]->swapchainColorAttachmentIndex = mBakedAttachmentsMapped[attachment.name]->colorAttachmentInfos.size() - 1;
    }
  }

  return *this;
}

sf::RenderGraphManager& sf::RenderGraphManager::rebuild() {
  mRenderGraphImageManager.destroy();

  mBakedAttachmentsMapped.clear();
  mBakedAttachments.clear();

  build();

  return *this;
}

sf::RenderGraphManager& sf::RenderGraphManager::setThreadWorkerRebuild() {
  executeFunction = [this]() { rebuild(); };

  return *this;
}

sf::RenderGraphManager& sf::RenderGraphManager::setThreadWorkerBuild() {
  executeFunction = [this]() { build(); }; 

  return *this;
}


sf::RenderGraphManager& sf::RenderGraphManager::run(vk::CommandBuffer& cmd, const std::string& name) {
  if(isThreadWorkerRunning())
    return *this;

  RenderGraphBakedAttachmentInfo* const baked = mBakedAttachmentsMapped[name];
  const RenderGraphManagerAttachment* const attachment = mRenderGraphAttachmentsMapped[name];

  if(!baked)
    SF_CLOG("ERR: No baked data for " << name << " pass");

  if(!attachment)
    SF_CLOG("ERR: No attachment data for " << name << " pass");

  for(const std::string& write : attachment->writeImageNames) {
    if(mRenderGraphImageManager[write].getImage() == nullptr)
      return *this;

    if(mRenderGraphImageManager[write].getSwapchainImageIndex() != ~0U) {
      mRenderGraphImageManager[write].transition(
        cmd, 
        vk::ImageLayout::eColorAttachmentOptimal, 
        vk::AccessFlagBits2::eColorAttachmentWrite, 
        vk::PipelineStageFlagBits2::eColorAttachmentOutput);
    }
    else {
      mRenderGraphImageManager.getSwapchainImage(sf::Window::imageIndex()).transition(
        cmd, 
        vk::ImageLayout::eColorAttachmentOptimal, 
        vk::AccessFlagBits2::eColorAttachmentWrite, 
        vk::PipelineStageFlagBits2::eColorAttachmentOutput);
    }
  }

  if(!attachment->depthImageName.empty()) {
    if(mRenderGraphImageManager[attachment->depthImageName].getImage() == nullptr)
      return *this;

    mRenderGraphImageManager[attachment->depthImageName].transition(
      cmd, 
      vk::ImageLayout::eDepthStencilAttachmentOptimal, 
      vk::AccessFlagBits2::eDepthStencilAttachmentRead | vk::AccessFlagBits2::eDepthStencilAttachmentWrite, 
      vk::PipelineStageFlagBits2::eEarlyFragmentTests);
  }

  if(baked->swapchainColorAttachmentIndex != ~0U)
    baked->colorAttachmentInfos[baked->swapchainColorAttachmentIndex].setImageView(mRenderGraphImageManager.getSwapchainImage(Window::imageIndex()).getView());

  vk::RenderingInfo renderingInfo;
  renderingInfo
    .setColorAttachmentCount(static_cast<uint32_t>(baked->colorAttachmentInfos.size()))
    .setPColorAttachments(baked->colorAttachmentInfos.data())
    .setPDepthAttachment(attachment->depthImageName.empty() ? nullptr : &baked->depthAttachmentInfo)
    .setViewMask(0)
    .setLayerCount(1)
    .setPStencilAttachment(nullptr)
    .setRenderArea(vk::Rect2D({0, 0}, vk::Extent2D(Window::width(), Window::height())));

  cmd.beginRendering(&renderingInfo);

  attachment->execute(cmd, mRenderGraphImageManager);

  cmd.endRendering();

  for(const std::string& write : attachment->writeImageNames) {
    if(mRenderGraphImageManager[write].getSwapchainImageIndex() == ~0U) {
      mRenderGraphImageManager[write].transition(
        cmd, 
        vk::ImageLayout::eShaderReadOnlyOptimal, 
        vk::AccessFlagBits2::eShaderRead, 
        vk::PipelineStageFlagBits2::eFragmentShader);
    }
    else {
      mRenderGraphImageManager.getSwapchainImage(sf::Window::imageIndex()).transition(
        cmd, 
        vk::ImageLayout::ePresentSrcKHR, 
        vk::AccessFlagBits2::eNone, 
        vk::PipelineStageFlagBits2::eBottomOfPipe);
    }
  }

  if(!attachment->depthImageName.empty()) {
    mRenderGraphImageManager[attachment->depthImageName].transition(
      cmd, 
      vk::ImageLayout::eShaderReadOnlyOptimal, 
      vk::AccessFlagBits2::eShaderRead, 
      vk::PipelineStageFlagBits2::eFragmentShader);
  }

  return *this;
}

sf::RenderGraphImageManager& sf::RenderGraphManager::getImageManager() {
  return mRenderGraphImageManager;
}

void sf::RenderGraphManager::destroy() {
  mRenderGraphImageManager.destroy();

  mBakedAttachments.clear();
  mBakedAttachmentsMapped.clear();

  mRenderGraphAttachmentsMapped.clear();
  mRenderGraphAttachments.clear();

  mImagesInfo.clear();
}
