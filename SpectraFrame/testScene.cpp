#include "Core/sfWindow.h"
#include "Rendering/sfRenderGraph.h"
#include "Scene/sfScene.h"
#include "Core/sfLogger.h"
#include "stb_image.h"
#include "Rendering/sfPipeline.h"
#include "Core/sfVkCommon.h"

#include "Math/sfQuaterion.h"

#include "Core/sfPLYLoader.h"
#include "Core/sfCommon.h"

class TestScene : public sf::Scene {
public:
  TestScene() : sf::Scene("TestScene", true) {}

  sf::RenderGraphManager renderGraphManager;

  vk::PipelineLayout layout;
  vk::PipelineLayout layout2;

  sf::GraphicsPipeline pipeline;
  sf::GraphicsPipeline pipeline2;

  vk::DescriptorSet pipeline2Set;
  vk::DescriptorPool pipeline2Pool;
  vk::DescriptorSetLayout pipeline2SetLayout;

  sf::VertexBuffer buffer;

  bool updateDescriptor = true;

  void createRenderPass() {
    renderGraphManager
      .addImage("Geometry", vk::Format::eR8G8B8A8Srgb)
      .addDepthImage("GeometryDepth")
      .addAttachment(sf::RenderGraphManagerAttachment{"GeometryPass", 
        [this](vk::CommandBuffer& cmd, sf::RenderGraphImageManager& imageManager) {
          if(!pipeline.isThreadWorkerRunning() && pipeline.getPipeline() != nullptr) {
            cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.getPipeline());

            vk::Viewport viewport{0.0f, 0.0f, (float)sf::Window::width(), (float)sf::Window::height(), 0.0f, 1.0f};
            vk::Rect2D scissor{{0, 0}, vk::Extent2D(sf::Window::width(), sf::Window::height())};

            vk::DeviceSize offset[] = {0};

            cmd.setViewport(0, 1, &viewport);
            cmd.setScissor(0, 1, &scissor);
            cmd.bindVertexBuffers(0, 1, &buffer.getBuffer(), offset);

            cmd.draw(3, 1, 0, 0);
          }
        },
         {"Geometry"}, "GeometryDepth"})

      .addAttachment(sf::RenderGraphManagerAttachment{"LightingPass", 
        [this](vk::CommandBuffer& cmd, sf::RenderGraphImageManager& imageManager) {
          if(!pipeline2.isThreadWorkerRunning() && pipeline2.getPipeline() != nullptr) {
            cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline2.getPipeline());

            vk::Viewport viewport{0.0f, 0.0f, (float)sf::Window::width(), (float)sf::Window::height(), 0.0f, 1.0f};
            vk::Rect2D scissor{{0, 0}, vk::Extent2D(sf::Window::width(), sf::Window::height())};

            cmd.setViewport(0, 1, &viewport);
            cmd.setScissor(0, 1, &scissor);

            cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, layout2, 0, 1, &pipeline2Set, 0, nullptr);

            cmd.draw(6, 1, 0, 0);
          }
        },
         {"Swapchain_0"}, std::string()});

    uint32_t swapchainImagesIter = 0;
    for(const vk::Image& image : sf::Window::images()) {
      renderGraphManager.addSwapchainImage("Swapchain_" + std::to_string(swapchainImagesIter), image, swapchainImagesIter);

      swapchainImagesIter++;
    }

    sf::Window::threadPool().addTask(&renderGraphManager);
    //renderGraphManager.build();
  }

  vk::Format pipelineColorAttachment;
  vk::Format pipeline2ColorAttachment;

  void updateWriteDescriptor() {
    vk::DescriptorImageInfo imageInfo {
      renderGraphManager.getImageManager()["Geometry"].getSampler(), 
      renderGraphManager.getImageManager()["Geometry"].getView(), 
      vk::ImageLayout::eShaderReadOnlyOptimal
    };

    vk::WriteDescriptorSet writeSet{};
    writeSet
      .setDescriptorCount(1)
      .setDstBinding(0)
      .setDstSet(pipeline2Set)
      .setPImageInfo(&imageInfo)
      .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
      .setDstArrayElement(0);

    sf::Vulkan::device().updateDescriptorSets(1, &writeSet, 0, nullptr);
  }

  void createPipeline() {
    pipelineColorAttachment = vk::Format::eR8G8B8A8Srgb;
    pipeline2ColorAttachment = sf::Vulkan::surfaceFormat().format;

    vk::PipelineLayoutCreateInfo layoutInfo;

    if(sf::Vulkan::device().createPipelineLayout(&layoutInfo, nullptr, &layout) != vk::Result::eSuccess)
      SF_CLOG("ERR: Cannot create pipeline layout");

    if(std::filesystem::is_directory(std::filesystem::current_path() / "SpectraFrame"))
      std::filesystem::current_path(std::filesystem::current_path() / "SpectraFrame");

    pipeline
      .setPipelineLayout(&layout)
      .setBindings({(vk::VertexInputBindingDescription){0, sizeof(float) * 5, vk::VertexInputRate::eVertex}})
      .setAttributes({
        (vk::VertexInputAttributeDescription){0, 0, vk::Format::eR32G32Sfloat, 0},
        (vk::VertexInputAttributeDescription){1, 0, vk::Format::eR32G32B32Sfloat, sizeof(float) * 2}
      })
      .setDynamicStates({
        vk::DynamicState::eViewport,
        vk::DynamicState::eScissor
      })
      .setShader(vk::ShaderStageFlagBits::eVertex, std::filesystem::current_path() / "../g_buffer_vert.spv")
      .setShader(vk::ShaderStageFlagBits::eFragment, std::filesystem::current_path() / "../g_buffer_frag.spv")
      .enableBlend({false})
      .setRenderingInfo(vk::PipelineRenderingCreateInfo{0, 1, &pipelineColorAttachment, sf::Vulkan::depthFormat()})
      .createPipelineCache({});

    sf::Window::threadPool().addTask(&pipeline);

    vk::DescriptorPoolSize poolSize{vk::DescriptorType::eCombinedImageSampler, 1};

    vk::DescriptorPoolCreateInfo poolInfo{};
    poolInfo
      .setFlags(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet)
      .setPoolSizeCount(1)
      .setPPoolSizes(&poolSize)
      .setMaxSets(1);

    sf::Vulkan::device().createDescriptorPool(&poolInfo, nullptr, &pipeline2Pool);

    vk::DescriptorSetLayoutBinding binding{0, vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment};

    vk::DescriptorSetLayoutCreateInfo setLayoutInfo{};
    setLayoutInfo
      .setBindingCount(1)
      .setPBindings(&binding);

    sf::Vulkan::device().createDescriptorSetLayout(&setLayoutInfo, nullptr, &pipeline2SetLayout);

    vk::DescriptorSetAllocateInfo allocInfo{};
    allocInfo
      .setDescriptorPool(pipeline2Pool)
      .setDescriptorSetCount(1)
      .setPSetLayouts(&pipeline2SetLayout);

    sf::Vulkan::device().allocateDescriptorSets(&allocInfo, &pipeline2Set); 

    layoutInfo
      .setSetLayoutCount(1)
      .setPSetLayouts(&pipeline2SetLayout);

    sf::Vulkan::device().createPipelineLayout(&layoutInfo, nullptr, &layout2);

    pipeline2
      .setDynamicStates({vk::DynamicState::eViewport, vk::DynamicState::eScissor})
      .setPipelineLayout(&layout2)
      .setShader(vk::ShaderStageFlagBits::eVertex, std::filesystem::current_path() / "../light_vert.spv")
      .setShader(vk::ShaderStageFlagBits::eFragment, std::filesystem::current_path() / "../light_frag.spv")
      .enableDepthTest(false)
      .enableBlend({false})
      .setRenderingInfo(vk::PipelineRenderingCreateInfo{0, 1, &pipeline2ColorAttachment})
      .createPipelineCache({});

    sf::Window::threadPool().addTask(&pipeline2);
  }

  virtual void start() override {
    SF_CDEBUG("Started test scene");
   
    std::vector<float> data = {
      0.0f, 0.5f, 1.0f, 0.0f, 0.0f,
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
      0.5f, -0.5f, 0.0f, 0.0f, 1.0f
    };

    buffer.update(sf::Window::commandPool(), sf::Vulkan::graphicsQueue(), data);

    createRenderPass();

    createPipeline();

    SF_CDEBUG("Start ended");
  }

  virtual void update() override {
    if(!renderGraphManager.isThreadWorkerRunning() && updateDescriptor) {
      updateDescriptor = false;
      updateWriteDescriptor();
    }

    if(sf::Window::sIsResized()) {
      renderGraphManager.rebuild();

      return;
    }

    sf::Window::beginFrame();

    renderGraphManager.run(sf::Window::currentCommandBuffer(), "GeometryPass");

    renderGraphManager.run(sf::Window::currentCommandBuffer(), "LightingPass");

    sf::Window::endFrame(); 
  }

  virtual void lateUpdate() override {
    
  }

  virtual void end() override {
    SF_CDEBUG("Ended test scene");
   
    renderGraphManager.destroy();

    sf::Vulkan::device().destroyPipelineLayout(layout);
    sf::Vulkan::device().destroyPipelineLayout(layout2);

    buffer.destroy();

    pipeline.destroyCache();
    pipeline2.destroyCache();

    pipeline.destroy();
    pipeline2.destroy();

    sf::Vulkan::device().freeDescriptorSets(pipeline2Pool, 1, &pipeline2Set);
    sf::Vulkan::device().destroyDescriptorSetLayout(pipeline2SetLayout);
    sf::Vulkan::device().destroyDescriptorPool(pipeline2Pool);

    SF_CDEBUG("All test scene objects has been destroyed");
  }
} G_TEST_SCENE;
