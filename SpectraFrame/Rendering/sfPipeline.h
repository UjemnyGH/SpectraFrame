#pragma once
#ifndef _SPECTRAFRAME_PIPELINE_
#define _SPECTRAFRAME_PIPELINE_

#include "../Core/sfWindow.h"
#include <unordered_map>
#include <filesystem>

namespace sf {
  class GraphicsPipeline : public ThreadWorker {
  private:
    vk::Pipeline mPipeline;
    vk::PipelineCache mPipelineCache;

    vk::RenderPass mRenderPass;
    vk::PipelineLayout* mPipelineLayoutPtr;
    
    std::unordered_map<vk::ShaderStageFlagBits, std::filesystem::path> mShaders;
    std::vector<vk::DynamicState> mDynamicStates;
    std::vector<vk::VertexInputAttributeDescription> mVertexInputAttributes;
    std::vector<vk::VertexInputBindingDescription> mVertexInputBindings;

    bool mEnableDepthTest = true;
    std::vector<bool> mEnableBlend;
    bool mEnableTessellation = false;

    vk::PipelineRenderingCreateInfo mRenderingInfo;

  public:
    GraphicsPipeline();

    GraphicsPipeline& setRenderPass(vk::RenderPass renderPass);

    GraphicsPipeline& setRenderingInfo(const vk::PipelineRenderingCreateInfo& renderingInfo);

    GraphicsPipeline& setPipelineLayout(vk::PipelineLayout* const pLayout);

    GraphicsPipeline& setShader(vk::ShaderStageFlagBits shaderStage, const std::filesystem::path& pathToShader);

    GraphicsPipeline& clearShaders();

    GraphicsPipeline& setDynamicStates(const std::vector<vk::DynamicState>& states);

    GraphicsPipeline& setAttributes(const std::vector<vk::VertexInputAttributeDescription>& attributes);

    GraphicsPipeline& setBindings(const std::vector<vk::VertexInputBindingDescription>& bindings);

    GraphicsPipeline& enableDepthTest(bool enable);

    GraphicsPipeline& enableBlend(const std::vector<bool>& enable);

    GraphicsPipeline& enableTessellation(bool enable);

    GraphicsPipeline& createPipelineCache(const std::vector<uint8_t>& cachePreinitializedData);

    vk::PipelineCache& getPipelineCache();

    vk::Pipeline& getPipeline();

    void build();

    void destroyCache();

    void destroy();
  };

  class ComputePipeline : public ThreadWorker {
  private:
    vk::Pipeline mPipeline;
    vk::PipelineCache mPipelineCache;

    vk::PipelineLayout* mPipelineLayoutPtr;

    std::filesystem::path mComputeShaderPath;

  public:
    ComputePipeline();

    ComputePipeline& setPipelineLayout(vk::PipelineLayout* pLayout);

    ComputePipeline& setShader(const std::filesystem::path& pathToComputeShader);

    ComputePipeline& createPipelineCache(const std::vector<uint8_t>& cachePreinitializedData);

    vk::PipelineCache& getPipelineCache();

    vk::Pipeline& getPipeline();

    void build();

    void destroyCache();

    void destroy();
  };
}

#endif
