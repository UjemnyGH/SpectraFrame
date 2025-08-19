#include "sfPipeline.h"
#include <fstream>
#include "../Core/sfLogger.h"

sf::GraphicsPipeline::GraphicsPipeline() : ThreadWorker(6) {
  mPipelineCache = nullptr;
  mPipelineLayoutPtr = nullptr;
  mRenderPass = nullptr;
  
  mEnableBlend.clear();
  mShaders.clear();
  mDynamicStates.clear();
  mVertexInputAttributes.clear();
  mVertexInputBindings.clear();

  executeFunction = [this](){ build(); };
}

sf::GraphicsPipeline& sf::GraphicsPipeline::setRenderPass(vk::RenderPass renderPass) {
  mRenderPass = renderPass;

  return *this;
}

sf::GraphicsPipeline& sf::GraphicsPipeline::setRenderingInfo(const vk::PipelineRenderingCreateInfo& renderingInfo) {
  mRenderingInfo = renderingInfo;

  return *this;
}

sf::GraphicsPipeline& sf::GraphicsPipeline::setPipelineLayout(vk::PipelineLayout* const pLayout) {
  mPipelineLayoutPtr = pLayout;

  return *this;
}

sf::GraphicsPipeline& sf::GraphicsPipeline::setShader(vk::ShaderStageFlagBits shaderStage, const std::filesystem::path& pathToShader) {
  mShaders[shaderStage] = pathToShader;

  return *this;
}

sf::GraphicsPipeline& sf::GraphicsPipeline::clearShaders() {
  mShaders.clear();

  return *this;
}

sf::GraphicsPipeline& sf::GraphicsPipeline::setDynamicStates(const std::vector<vk::DynamicState>& states) {
  mDynamicStates = states;

  return *this;
}

sf::GraphicsPipeline& sf::GraphicsPipeline::setAttributes(const std::vector<vk::VertexInputAttributeDescription>& attributes) {
  mVertexInputAttributes = attributes;

  return *this;
}

sf::GraphicsPipeline& sf::GraphicsPipeline::setBindings(const std::vector<vk::VertexInputBindingDescription>& bindings) {
  mVertexInputBindings = bindings;

  return *this;
}

sf::GraphicsPipeline& sf::GraphicsPipeline::enableDepthTest(bool enable) {
  mEnableDepthTest = enable;

  return *this;
}

sf::GraphicsPipeline& sf::GraphicsPipeline::enableBlend(const std::vector<bool>& enable) {
  mEnableBlend = enable;

  return *this;
}

sf::GraphicsPipeline& sf::GraphicsPipeline::enableTessellation(bool enable) {
  mEnableTessellation = enable;

  return *this;
}

sf::GraphicsPipeline& sf::GraphicsPipeline::createPipelineCache(const std::vector<uint8_t>& cachePreinitializedData) {
  // Fillup info if there is loaded data to initialize cache from, if not then nullptr it
  vk::PipelineCacheCreateInfo cacheInfo{};
  cacheInfo
    .setInitialDataSize(static_cast<uint32_t>(cachePreinitializedData.size()))
    .setPInitialData(cachePreinitializedData.empty() ? nullptr : cachePreinitializedData.data());

  if(Vulkan::device().createPipelineCache(&cacheInfo, nullptr, &mPipelineCache) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create graphics pipeline cache");

  return *this;
}

vk::PipelineCache& sf::GraphicsPipeline::getPipelineCache() {
  return mPipelineCache;
}

vk::Pipeline& sf::GraphicsPipeline::getPipeline() {
  return mPipeline;
}

void sf::GraphicsPipeline::build() {
  // Build struct containing all informations how data are gonna be passed to pipeline
  vk::PipelineVertexInputStateCreateInfo vertexInputInfo{};
  vertexInputInfo
    .setPVertexAttributeDescriptions(mVertexInputAttributes.data())
    .setPVertexBindingDescriptions(mVertexInputBindings.data())
    .setVertexAttributeDescriptionCount(static_cast<uint32_t>(mVertexInputAttributes.size()))
    .setVertexBindingDescriptionCount(static_cast<uint32_t>(mVertexInputBindings.size()));

  // Info about how this passed data would be assembled
  vk::PipelineInputAssemblyStateCreateInfo assemblyInfo{};
  assemblyInfo
    .setPrimitiveRestartEnable(vk::False)
    .setTopology(mEnableTessellation ? vk::PrimitiveTopology::ePatchList :vk::PrimitiveTopology::eTriangleList);

  // Rasterization info
  vk::PipelineRasterizationStateCreateInfo rasterizationInfo{};
  rasterizationInfo
    .setCullMode(vk::CullModeFlagBits::eNone)
    .setPolygonMode(vk::PolygonMode::eFill)
    .setLineWidth(1.0f)
    .setRasterizerDiscardEnable(vk::False)
    .setFrontFace(vk::FrontFace::eClockwise);

  // Here we add all blend attachments (same amount as render pass attachments)
  std::vector<vk::PipelineColorBlendAttachmentState> blendAttachments;
  blendAttachments.reserve(mEnableBlend.size());
 
  // Choose which one would have enabled blending and which not
  for(bool enableBlend : mEnableBlend) {
    vk::PipelineColorBlendAttachmentState attachment{};
    attachment
      .setColorBlendOp(vk::BlendOp::eAdd)
      .setDstColorBlendFactor(vk::BlendFactor::eZero)
      .setSrcColorBlendFactor(vk::BlendFactor::eOne)
      .setAlphaBlendOp(vk::BlendOp::eAdd)
      .setDstAlphaBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha)
      .setSrcAlphaBlendFactor(vk::BlendFactor::eSrcAlpha)
      .setBlendEnable(enableBlend)
      .setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);

    blendAttachments.emplace_back(attachment);
  }

  // Put all attachment into another struct
  vk::PipelineColorBlendStateCreateInfo colorBlendInfo{};
  colorBlendInfo
    .setAttachmentCount(static_cast<uint32_t>(blendAttachments.size()))
    .setPAttachments(blendAttachments.data());

  // Make struct about viewport and scissor state (these must be dynamic, dont bother making them here)
  vk::PipelineViewportStateCreateInfo viewportInfo{};
  viewportInfo
    .setViewportCount(1)
    .setScissorCount(1);

  // Depth info on demand, if enabled pipeline make depth tests
  vk::PipelineDepthStencilStateCreateInfo depthInfo{};
  depthInfo
    .setDepthCompareOp(mEnableDepthTest ? vk::CompareOp::eLess : vk::CompareOp::eAlways)
    .setDepthTestEnable(mEnableDepthTest)
    .setDepthWriteEnable(mEnableDepthTest)
    .setDepthBoundsTestEnable(vk::False)
    .setMinDepthBounds(0.0f)
    .setMaxDepthBounds(1.0f)
    .setStencilTestEnable(vk::False)
    .setFront({})
    .setBack({});

  // TODO: Multisampling
  vk::PipelineMultisampleStateCreateInfo multisamplingInfo{};
  multisamplingInfo
    .setRasterizationSamples(vk::SampleCountFlagBits::e1);

  // Dynamic states info, there should be at least 2 dynamic states: for viewport and for scissor
  vk::PipelineDynamicStateCreateInfo dynamicInfo{};
  dynamicInfo
    .setDynamicStateCount(static_cast<uint32_t>(mDynamicStates.size()))
    .setPDynamicStates(mDynamicStates.data());

  // TODO: Tessellation
  vk::PipelineTessellationStateCreateInfo tessellationInfo{};
  tessellationInfo
    // TODO: Possibly set this custom value to provide more flexibility
    .setPatchControlPoints(mEnableTessellation ? 3 : 0);

  // All shader modules contained to destroy after creating pipeline
  std::vector<vk::ShaderModule> shaders;
  shaders.reserve(mShaders.size());

  // All shader stages info
  std::vector<vk::PipelineShaderStageCreateInfo> shaderInfo;
  shaderInfo.reserve(mShaders.size());

  // Iterate through all added shaders
  for(auto [stage, path] : mShaders) {
    // Open them
    std::ifstream shaderSrc(path, std::ios::binary | std::ios::ate);

    if(!shaderSrc.is_open()) {
      SF_CLOG("ERR: Cannot open shader " << path.generic_string());
    
      continue;
    }

    if(shaderSrc.bad()) {
      SF_CLOG("ERR: Shader " << path.generic_string() << " bad");

      continue;
    }

    size_t len = shaderSrc.tellg();
    shaderSrc.seekg(0, std::ios::beg);

    // Read shader source file
    std::vector<uint8_t> shaderSrcBuff(len);
    shaderSrc.read((char*)shaderSrcBuff.data(), len);

    shaderSrc.close();

    // Make module 
    vk::ShaderModuleCreateInfo shaderModuleInfo{};
    shaderModuleInfo
      .setPCode(reinterpret_cast<const uint32_t*>(shaderSrcBuff.data()))
      .setCodeSize(static_cast<uint32_t>(shaderSrcBuff.size()));

    vk::ShaderModule module;

    if(Vulkan::device().createShaderModule(&shaderModuleInfo, nullptr, &module) != vk::Result::eSuccess)
      SF_CLOG("ERR: Cannot create shader " << path.generic_string());

    // Push module to be destroyed later
    shaders.push_back(module);

    // Make full shader info
    shaderInfo.emplace_back(vk::PipelineShaderStageCreateInfo({}, stage, module, "main"));
  }

  // Fill entire graphics pipeline creation info with everything above 
  vk::GraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo
    .setPNext(&mRenderingInfo)
    .setBasePipelineIndex(-1)
    .setBasePipelineHandle(nullptr)
    .setLayout(*mPipelineLayoutPtr)
    .setPVertexInputState(&vertexInputInfo)
    .setPInputAssemblyState(&assemblyInfo)
    .setPRasterizationState(&rasterizationInfo)
    .setPColorBlendState(&colorBlendInfo)
    .setPViewportState(&viewportInfo)
    .setPDepthStencilState(&depthInfo)
    .setPMultisampleState(&multisamplingInfo)
    .setPDynamicState(&dynamicInfo)
    .setPTessellationState(mEnableTessellation ? &tessellationInfo : nullptr)
    .setRenderPass(mRenderPass)
    .setSubpass(0)
    .setStageCount(static_cast<uint32_t>(shaderInfo.size()))
    .setPStages(shaderInfo.data());

  // Create pipeline
  if(Vulkan::device().createGraphicsPipelines(mPipelineCache, 1, &pipelineInfo, nullptr, &mPipeline) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create graphics pipeline");

  // And destroy shader modules, as we dont need them anymore
  for(vk::ShaderModule mod : shaders) {
    Vulkan::device().destroyShaderModule(mod, nullptr);
  }
}

void sf::GraphicsPipeline::destroyCache() {
  if(mPipelineCache)
    Vulkan::device().destroyPipelineCache(std::exchange(mPipelineCache, nullptr), nullptr);
}

void sf::GraphicsPipeline::destroy() {
  if(mPipeline)
    Vulkan::device().destroyPipeline(std::exchange(mPipeline, nullptr), nullptr);
}

sf::ComputePipeline::ComputePipeline() : ThreadWorker(6) {
  mPipelineCache = nullptr;
  mPipelineLayoutPtr = nullptr;

  executeFunction = [this](){ build(); };
}

sf::ComputePipeline& sf::ComputePipeline::setPipelineLayout(vk::PipelineLayout* pLayout) {
  mPipelineLayoutPtr = pLayout;

  return *this;
}

sf::ComputePipeline& sf::ComputePipeline::setShader(const std::filesystem::path& pathToComputeShader) {
  mComputeShaderPath = pathToComputeShader;

  return *this;
}

sf::ComputePipeline& sf::ComputePipeline::createPipelineCache(const std::vector<uint8_t>& cachePreinitializedData) {
  // Load cache data based on their existance
  vk::PipelineCacheCreateInfo cacheInfo{};
  cacheInfo
    .setInitialDataSize(static_cast<uint32_t>(cachePreinitializedData.size()))
    .setPInitialData(cachePreinitializedData.empty() ? nullptr : cachePreinitializedData.data());

  if(Vulkan::device().createPipelineCache(&cacheInfo, nullptr, &mPipelineCache) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create compute pipeline cache");

  return *this;
}

vk::PipelineCache& sf::ComputePipeline::getPipelineCache() {
  return mPipelineCache;
}

vk::Pipeline& sf::ComputePipeline::getPipeline() {
  return mPipeline;
}

void sf::ComputePipeline::build() {
  // Open shader source
  std::ifstream shaderSrc(mComputeShaderPath, std::ios::binary | std::ios::ate);

  if(!shaderSrc.is_open()) {
    SF_CLOG("ERR: Cannot open file " << mComputeShaderPath.generic_string());

    return;
  }

  if(shaderSrc.bad()) {
    SF_CLOG("ERR: File " << mComputeShaderPath.generic_string() << " bad");

    return;
  }

  // Get file size
  size_t len = shaderSrc.tellg();
  shaderSrc.seekg(0, std::ios::beg);

  // Read compute shader source file
  std::vector<uint8_t> shaderSrcBuff(len);
  shaderSrc.read((char*)shaderSrcBuff.data(), len);

  shaderSrc.close();

  // Make module out of loaded source
  vk::ShaderModuleCreateInfo moduleInfo{};
  moduleInfo
    .setCodeSize(static_cast<uint32_t>(shaderSrcBuff.size()))
    .setPCode(reinterpret_cast<const uint32_t*>(shaderSrcBuff.data()));

  vk::ShaderModule module;

  if(Vulkan::device().createShaderModule(&moduleInfo, nullptr, &module) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create compute shader module");

  // Make pipeline shader stage
  vk::PipelineShaderStageCreateInfo shaderInfo{};
  shaderInfo
    .setPName("main")
    .setStage(vk::ShaderStageFlagBits::eCompute)
    .setModule(module);

  // Fill pipeline data struct
  vk::ComputePipelineCreateInfo computeInfo{};
  computeInfo
    .setBasePipelineHandle(nullptr)
    .setBasePipelineIndex(-1)
    .setLayout(*mPipelineLayoutPtr)
    .setStage(shaderInfo);

  // Create pipeline
  if(Vulkan::device().createComputePipelines(mPipelineCache, 1, &computeInfo, nullptr, &mPipeline) != vk::Result::eSuccess)
    SF_CLOG("ERR: Cannot create compute pipeline");

  // Destroy shader module as it is no longer needed
  Vulkan::device().destroyShaderModule(module, nullptr);
}

void sf::ComputePipeline::destroyCache() {
  if(mPipelineCache)
    Vulkan::device().destroyPipelineCache(std::exchange(mPipelineCache, nullptr), nullptr);
}

void sf::ComputePipeline::destroy() {
  if(mPipeline)
    Vulkan::device().destroyPipeline(std::exchange(mPipeline, nullptr), nullptr);
}

