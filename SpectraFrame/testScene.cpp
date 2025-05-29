#include "Core/sfWindow.h"
#include "Rendering/sfRenderPass.h"
#include "Scene/sfScene.h"
#include "Core/sfLogger.h"
#include "stb_image.h"
#include "Core/sfSwapchain.h"
#include "Rendering/sfRenderPassImages.h"

#include "Math/sfQuaterion.h"

#include "Core/sfPLYLoader.h"
#include "Core/sfCommon.h"

class JustRender : public sf::RenderPass {
public:
  JustRender() : sf::RenderPass(0) {}

  virtual void execute(vk::CommandBuffer& cmd) override {
    
  }
};

class TestScene : public sf::Scene {
public:
  TestScene() : sf::Scene("TestScene", true) {}

  JustRender pass;
  std::vector<vk::Framebuffer> framebuffers;

  vk::Pipeline pipeline;
  vk::PipelineLayout layout;

  vk::CommandPool pool;
  std::vector<vk::CommandBuffer> commandBuffers;

  sf::DepthImage depth;

  virtual void start() override {
    SF_CDEBUG("Started test scene");
    
    sf::createVkCommandPool(pool, sf::Vulkan::graphicsQueueIndex());

    depth.create(vk::Extent3D(sf::Window::sWindowInstancePtr->getWidth(), sf::Window::sWindowInstancePtr->getHeight(), 1), pool);

    pass.addAttachment(sf::Vulkan::surfaceFormat().format, vk::ImageLayout::ePresentSrcKHR);
    pass.addAttachment(sf::Vulkan::depthFormat(), vk::ImageLayout::eDepthStencilAttachmentOptimal);

    pass.build();

    framebuffers.resize(sf::Window::sWindowInstancePtr->getImages().size());
    commandBuffers.resize(framebuffers.size());

    for(uint32_t i = 0; i < framebuffers.size(); i++) {
      std::vector<vk::ImageView> attachments = {
        sf::Window::sWindowInstancePtr->getImageViews()[i],
        depth.getView()
      };

      vk::FramebufferCreateInfo framebufferInfo{};
      framebufferInfo
        .setAttachmentCount(static_cast<uint32_t>(attachments.size()))
        .setPAttachments(attachments.data())
        .setRenderPass(pass.getRenderPass())
        .setLayers(1)
        .setWidth(sf::Window::sWindowInstancePtr->getWidth())
        .setHeight(sf::Window::sWindowInstancePtr->getHeight());

      if(sf::Vulkan::device().createFramebuffer(&framebufferInfo, nullptr, &framebuffers[i]) != vk::Result::eSuccess)
        SF_CLOG("ERR: Cannot create framebuffer");

      sf::allocateCommandBuffer(commandBuffers[i], pool);
    }
  }

  virtual void update() {
    vk::CommandBufferBeginInfo beginInfo{};

    if(commandBuffers[sf::Window::sWindowInstancePtr->getCurrentImageIndex()].begin(&beginInfo) != vk::Result::eSuccess)
      SF_CLOG("ERR: Cannot begin command buffer");

    pass.run(commandBuffers[sf::Window::sWindowInstancePtr->getCurrentImageIndex()], framebuffers[sf::Window::sWindowInstancePtr->getCurrentImageIndex()]);

    commandBuffers[sf::Window::sWindowInstancePtr->getCurrentImageIndex()].end();

    vk::CommandBufferSubmitInfo cmdInfo;
    cmdInfo
      .setCommandBuffer(commandBuffers[sf::Window::sWindowInstancePtr->getCurrentImageIndex()]);

    vk::SemaphoreSubmitInfo waitInfo;
    waitInfo
      .setSemaphore(sf::Window::sWindowInstancePtr->getCurrentImageAvailableSemaphore())
      .setStageMask(vk::PipelineStageFlagBits2::eTopOfPipe);
    
    vk::SemaphoreSubmitInfo sigInfo;
    sigInfo
      .setSemaphore(sf::Window::sWindowInstancePtr->getCurrentRenderFinishedSemaphore())
      .setStageMask(vk::PipelineStageFlagBits2::eTopOfPipe);

    vk::SubmitInfo2 submitInfo{};
    submitInfo
      .setCommandBufferInfoCount(1)
      .setPCommandBufferInfos(&cmdInfo)
      .setWaitSemaphoreInfoCount(1)
      .setPWaitSemaphoreInfos(&waitInfo)
      .setSignalSemaphoreInfoCount(1)
      .setPSignalSemaphoreInfos(&sigInfo);

    if(sf::Vulkan::graphicsQueue().submit2(1, &submitInfo, sf::Window::sWindowInstancePtr->getCurrentFence()) != vk::Result::eSuccess)
      SF_CLOG("ERR: Cannot submit command buffer");
  }

  virtual void lateUpdate() {
    
  }

  virtual void end() {
    SF_CDEBUG("Ended test scene");
    
    depth.destroy();

    pass.destroy();

    sf::Vulkan::device().freeCommandBuffers(pool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
    sf::Vulkan::device().destroyCommandPool(pool, nullptr);

    for(vk::Framebuffer fb : framebuffers) {
      sf::Vulkan::device().destroyFramebuffer(fb, nullptr);
    }
  }
} G_TEST_SCENE;
