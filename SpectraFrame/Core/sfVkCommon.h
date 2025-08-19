#pragma once
#ifndef _SPECTRAFRAME_VULKAN_COMMON_
#define _SPECTRAFRAME_VULKAN_COMMON_

#include <vulkan/vulkan.hpp>
#include "sfVkCore.h"
#include "sfLogger.h"

namespace sf {
  void commandBufferOneTimeBegin(vk::CommandBuffer& commandBuffer, vk::CommandPool& commandPool);

  void commandBufferOneTimeEnd(vk::CommandBuffer& commandBuffer, vk::CommandPool& commandPool, const vk::Queue submitQueue);

  uint32_t findImageMemoryType(vk::DeviceSize& rAllocationSize, vk::Image image, const vk::MemoryPropertyFlags properties);

  uint32_t findBufferMemoryType(vk::DeviceSize& rAllocationSize, vk::Buffer buffer, const vk::MemoryPropertyFlags properties);
  
  //
  // Below are functions wrapping all vulkan handles that are not worth to put inside separate class,
  // they just have to few parameters to bother wrapping them in classes, destroying this objects works
  // by calling sf::Vulkan::device().destroyXHandle(<vk::XHandle name>, nullptr)
  //
  
  /**
   * Simple wrap around creating fence
   */
  void createVkFence(vk::Fence& rFence, bool signaled = true);

  /**
   * Simple wrap around creating semaphore
   */
  void createVkSemaphore(vk::Semaphore& rSemaphore);

  void createVkCommandPool(vk::CommandPool& rCommandPool, const uint32_t queueIndex, const bool resetBuffers = true, const bool transient = true);

  void allocateCommandBuffer(vk::CommandBuffer& rCommandBuffer, vk::CommandPool commandPool, const bool primary = true);

  void createVkRenderPassImage(vk::Image& rImage, vk::DeviceMemory& rMemory, const vk::Extent3D imageExtent, const vk::Format imageFormat);

  void createVkImageView(vk::ImageView& rImageView, vk::Image image, const vk::Format format, const vk::ImageViewType viewType, const bool depthImageView = false);

  struct CopyBuffer {
    vk::CommandBuffer* pCommandBuffer;

    CopyBuffer& setCommandBuffer(vk::CommandBuffer& commandBuffer);

    CopyBuffer& copyBufferToBuffer(vk::Buffer& src, vk::Buffer& dst, const vk::DeviceSize srcDataSize);

    CopyBuffer& copyBufferToImage(vk::Buffer& src, vk::Image& dst, const vk::Extent3D imageExtent, const vk::ImageSubresourceLayers subresources);

    /**
     * @brief Images need to have same extent, layout and subresources to copy succesfully, otherwise use full version frtom command buffer
     * @param src 
     * @param dst 
     * @param imageExtent 
     * @param imageLayout 
     * @param subresources 
     * @return 
     */
    CopyBuffer& copyImageToImage(vk::Image& src, vk::Image& dst, const vk::Extent3D imageExtent, const vk::ImageLayout imageLayout, const vk::ImageSubresourceLayers subresources);
  };

  struct TransitionImage {
    vk::CommandBuffer* pCommandBuffer;
    vk::ImageMemoryBarrier imageMemoryBarrier;

    TransitionImage();

    TransitionImage& setCommandBuffer(vk::CommandBuffer& commandBuffer);

    TransitionImage& layouts(const vk::ImageLayout oldL, const vk::ImageLayout newL);

    TransitionImage& accessMask(const vk::AccessFlags srcMask, const vk::AccessFlags dstMask);

    TransitionImage& subresources(const vk::ImageSubresourceRange subresources);

    TransitionImage& transition(vk::Image& image, const vk::PipelineStageFlags srcStageMask, const vk::PipelineStageFlags dstStageMask);
  };

  class VertexBuffer {
  private:
    vk::Buffer mBuffer;
    vk::DeviceMemory mBufferMemory;
    size_t mBufferSize;

  public:
    ~VertexBuffer();

    vk::Buffer& getBuffer();

    size_t getBufferSize();

    /**
     * @brief If vertex buffer is not created or buffer sizes does not match provided data then destroys old buffer (if exist) and creates new updated one
     * @param commandPool 
     * @param submitQueue 
     * @param data 
     * @return 
     */
    template <class T>
    inline VertexBuffer& update(vk::CommandPool& commandPool, vk::Queue submitQueue, const std::vector<T>& data)  {
      if (mBufferSize != data.size() * sizeof(data[0]) || mBuffer == nullptr || mBufferMemory == nullptr) {
        destroy();

        mBufferSize = data.size() * sizeof(data[0]);

        vk::BufferCreateInfo bufferInfo;
        bufferInfo
          .setSize(mBufferSize)
          .setSharingMode(vk::SharingMode::eExclusive)
          .setUsage(vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst)
          .setQueueFamilyIndexCount(0);

        if(Vulkan::device().createBuffer(&bufferInfo, nullptr, &mBuffer) != vk::Result::eSuccess)
          SF_CLOG("ERR: Cannot create vertex buffer buffer");

        vk::MemoryAllocateInfo memAllocInfo;
        memAllocInfo
          .setMemoryTypeIndex(findBufferMemoryType(memAllocInfo.allocationSize, mBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal));

        if(Vulkan::device().allocateMemory(&memAllocInfo, nullptr, &mBufferMemory) != vk::Result::eSuccess)
          SF_CLOG("ERR: Cannot allocate vertex buffer memory");

        Vulkan::device().bindBufferMemory(mBuffer, mBufferMemory, 0);
      }

      void* pCopyData;

      vk::Buffer stageBuffer;
      vk::DeviceMemory stageMemory;

      vk::BufferCreateInfo stageBufferInfo;
      stageBufferInfo
        .setSize(mBufferSize)
        .setUsage(vk::BufferUsageFlagBits::eTransferSrc)
        .setSharingMode(vk::SharingMode::eExclusive)
        .setQueueFamilyIndexCount(0);

      if(Vulkan::device().createBuffer(&stageBufferInfo, nullptr, &stageBuffer) != vk::Result::eSuccess)
        SF_CLOG("ERR: Cannot create vertex stage buffer buffer");

      vk::MemoryAllocateInfo stageMemAllocInfo;
      stageMemAllocInfo
        .setMemoryTypeIndex(findBufferMemoryType(stageMemAllocInfo.allocationSize, stageBuffer, vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible));

      if(Vulkan::device().allocateMemory(&stageMemAllocInfo, nullptr, &stageMemory) != vk::Result::eSuccess)
        SF_CLOG("ERR: Cannot allocate vertex stage buffer memory");

      Vulkan::device().bindBufferMemory(stageBuffer, stageMemory, 0);

      pCopyData = Vulkan::device().mapMemory(stageMemory, 0, mBufferSize);

      memcpy(pCopyData, &data[0], mBufferSize);

      Vulkan::device().unmapMemory(stageMemory);

      vk::CommandBuffer oneTimeCopy;
  
      commandBufferOneTimeBegin(oneTimeCopy, commandPool);

      CopyBuffer copyRegion;
      copyRegion
        .setCommandBuffer(oneTimeCopy)
        .copyBufferToBuffer(stageBuffer, mBuffer, mBufferSize);

      commandBufferOneTimeEnd(oneTimeCopy, commandPool, submitQueue);

      Vulkan::device().freeMemory(stageMemory, nullptr);
      Vulkan::device().destroyBuffer(stageBuffer, nullptr);

      return *this;
    }

    void destroy();
  };

  class UniformBuffer {
  private:
    vk::Buffer mBuffer;
    vk::DeviceMemory mBufferMemory;
    size_t mBufferSize;

  public:
    ~UniformBuffer();

    vk::Buffer& getBuffer();

    size_t getBufferSize();

    UniformBuffer& create(const size_t size);

    UniformBuffer& map(void** ppDataPlace);

    UniformBuffer& unmap();

    void destroy();
  };

  class Sampler {
  private:
    vk::Image mImage;
    vk::DeviceMemory mImageMemory;
    vk::ImageView mImageView;
    vk::Sampler mSampler;

    uint32_t mMipmapLevels;
    vk::Extent3D mImageExtent;

  public:
    ~Sampler();

    vk::Image& getImage();

    vk::ImageView& getImageView();

    vk::Sampler& getSampler();

    vk::Extent3D getImageExtent();

    uint32_t getMipmapLevels();

    Sampler& create(vk::CommandPool& commandPool, const vk::Queue submitQueue, const uint8_t* const pixels, const size_t pixelsSize, const vk::Extent3D extent, const vk::Format imageFormat, const uint32_t mipLevels);

    void destroy();
  };

  class ShaderStorageBuffer {
  private:
    vk::Buffer mBuffer;
  vk::DeviceMemory mBufferMemory;
    size_t mBufferSize;

  public:
    ~ShaderStorageBuffer();

    vk::Buffer& getBuffer();

    uint32_t getBufferSize();

    ShaderStorageBuffer& update(void* pData, const size_t size);

    ShaderStorageBuffer& map(void** ppDataPlace);

    ShaderStorageBuffer& unmap();

    void destroy();
  };

  class Shader {
  private:
    vk::ShaderModule mShaderModule;
    vk::ShaderStageFlags mShaderStage;

  public:
    ~Shader();

    vk::ShaderModule& getShaderModule();

    vk::ShaderStageFlags getShaderStage();

    Shader& create(const std::vector<uint8_t>& shaderBinary, const vk::ShaderStageFlags shaderStage);

    void destroy();
  };

  class DepthImage {
  private:
    vk::Image mDepthImage;
    vk::DeviceMemory mDepthImageMemory;
    vk::ImageView mDepthImageView;

  public:
    ~DepthImage();

    vk::Image& getImage();

    vk::DeviceMemory& getMemory();

    vk::ImageView& getView();

    DepthImage& create(const vk::Extent3D extent, vk::CommandPool& pool);

    void destroy();
  };
}

#endif
